Ownership Transfer vs. std::move
================================

Ownership transfer is more powerful than the std::move operator.  That is
because ownership transfer can transfer ownership of an entire object via
pointer-copy, while std::move still performs a field-by-field copy.[1]

The big difference in the two techniques is that the compiler takes care of
choosing the appropriate assignment operator or copy-constructor when std::move
is used.  In contrast, the onus is on the programmer to use ownership transfer
correctly.

Shortly after C++ was introduced, it became apparent that a lot of processing
time could be spent if one was not careful in the design or use of objects.
It was tempting to manipulate fully-constructed objects in programs, because the
compiler would insert the necessary destructor calls.  As long as you didn't
make the rookie mistake of passing a reference to an object out of the scope in
which it was defined, everything would "just work".

Unfortunately, a variable of a fully-constructed object type is associated with
storage sufficient to contain that object throughout its lifetime.  Thus,
associating new data with an existing object (through assignment) entails an
element-by-element copy.  This is necessary to overwrite the contents of the
existing object.

The problem arises often when a class is written such that object manipulations
(e.g. binary operators) return fully-constructed objects.  The returned value is
an unnamed temporary: it must either be supplied with a name, or it will go out
of scope and be reclaimed, to no effect.[2]  The two options for applying a name
are to assign the result to an existing variable (overwriting it), or to
initialize a new variable using the temporary value.  Initialization calls the
copy-constructor, and assignment and copy-construction usually share much of the
same code.  That is, the copy cannot be avoided by using initialization in place
of assignment.[3]

Programmers' reaction to this problem was to go back to using mostly pointers,
and performing the memory management themselves.  Which programmers suck at, so
schemes involving auto-pointers (objects that behave like pointers, but get
deleted automatically when they go out of scope) were introduced.

In the mean time, computers got a lot faster and the average programmer got a
lot sloppier, so they just settled down to using pointers (carefully) for large
data structures that needed to be moved around, passing in pointers or
references to objects that were to be updated (rather than passing back the
result), or just living with the performance hit associated with returning
fully-constructed objects.

But the performance hit still bothered library authors (like the guy who wrote
std::string), so the language designers thought about a way to avoid deep copies
(at least), and added that strategem to C++11.

The std::move solution
----------------------

The essential property of an unnamed temporary is that it must be used in the
expression in which it appears.  If not, then it will go out of scope and be
reclaimed.   Recognizing that the lifetime of an unnamed temporary is limited,
that use is tantamount to a "last chance".  Either it will be used by a callee
in the same expression or not.  If not, then it will be reclaimed.

This leads to the observation that a callee operating on an unnamed temporary
can do whatever it wants with that object, because it will *never be read again*
before it is reclaimed.  One could, for example, steal all the data owned by
that object and overwrite its contents with zeroes.  And that gets us one third
of the way there.

Another part of the problem is how to distinguish these unnamed temporaries
from objects whose integrity should be preserved.  The solution was to add a new
type modifier, spelled &&, that can be attached to a parameter to signify that
we know the passed-in object is an unnamed temporary.[4][5]

The final part of the solution is that the compiler is changed to do something
interesting with these rvalue references.  If there is a call to a function and
there are overloaded definitions of this function -- one with an rvalue
reference in one position and the other with a const reference -- the compiler
will preferentially bind the one with the rvalue-reference parameter only if the
operand is an expiring rvalue.  The programmer has to write two versions of the
function -- one that can steal the contents of the object, and another that will
not -- and the compiler handles the rest.

The Ownership Transfer Solution
-------------------------------

The ownership transfer solution suppors the transferrence of ownership of an
object through pointer copying.  In the case of a return value object, ownership
is transferred to the caller as part of the interface contract.  Rather than
relying on the compiler to choose the right interface and handle memory
management automatically, OTS relies on the programmer to get things right.  To
make this easier, OTS can be implemented as a coding convention.

The coding convention associates certain semantics and accompanying coding
patterns with parameter and return value type modifiers, as shown in the tables
below.[6]

        Return Value Semantics
    Type		Semantics
    T       	Value[7]
    T*      	Caller must take ownership
    T&      	Callee retains ownership; caller can modify
    const T&	Callee retains ownership; caller cannot modify

		Parameter Semantics
    Type		Semantics
    T			Value
	T*			(not used)
    T*&			Caller offers ownership; callee can steal
    T&          Caller retains ownership; callee can modify
    const T&    Caller retains ownership; callee cannot modify

You can see that the interesting case is the one where an object with pointer
type (T*) is bound to a parameter with the pointer-reference modifier.  As a
brief example, suppose we have a factory function (create()) that generates an
object of type T, and we have some container that will receive and take
ownership of such an object.  That situation would look like this:

  static T* T::create(...) { 
    T* result = new T(); /* set up result ... */; return result;
  }

  void Container::insert(T*& t) {
    Slot& slot = allocate_slot(); // Make space in my internal rep.
    slot.data = t; // Insert the object into my internal rep.
    t = 0; // Steal ownership of the passed in object here.
  }

  void example() {
    Container c;
    c.insert(T::create(/*args*/));
  }

Whether the call to c::insert() actually takes ownership of the passed-in T* is
a matter of convention.  In some code that I wrote for my C++ parser, I found it
was convenient to allow some routines the option, rather than forcing them to
always take ownership.  But one could just as well make stealing the rule, and
optional stealing the exception (suitably documented, of course).

If insert does not always take ownership of its operand, then the caller has to
take care of the case when it does not.  The above example must be rewritten as:

  void example() {
    Container c;
    T* item = T::create(/*args*/);
    c.insert(item);
    delete item;
  }

According to the C++ spec, the delete operator has a built-in null check, so
this will succeed without errror whether or not C::insert() adopts the item.

In the presence of an exceptions, one must also take care of finally
deleting an item should and exception intervene its allocation and adoption.
The example code then becomes:

  void example() {
    Container c;
    T* item = 0;
    try {
      item = T::create(/*args*/);
      /* Statements that may throw */
      c.insert(item);
    } catch (E& e) {
      /* Handle an E exception. */
      delete item;
    } catch (...) {
      /* Unhandled exception. */
      delete item;
      throw; // Re-throw because we didn't actually handle it.
    }
  }

In order to ensure that the item is deallocated properly, every catch clause
must delete it and there must always be a catch-all clause to delete it even in
the case that the exception goes unhandled (note that it is re-thrown).  All of
this extra text can be annoying, but an auto-pointer class can take care of the
problem for us.  If we expect exceptions, it is sufficient to write:

  void example() {
    std::unique_ptr<T> wrapped_item;
    wrapped_item.reset(T::create(/*args*/));
    c.insert(wrapped_item);
  }

Now, if an exception intervenes the creation and adoption of the unnamed T
object, the unique_ptr class will take care of deallocating it before it goes
out of scope.

--------------------

[1] In practice, this is most likely performed using a bulk copy like memcpy().
[2] Actually, one can perform further operations on the unnamed temporary, so
    creating it without naming it isn't necesarily a waste of electrical energy.
[3] Creating an object only to overwrite it is obviously wasteful, so should be
    avoided.
[4] The term coined by the C++ language mavens was "expiring r-value" or
    x-value.  The && type modifier is called an "rvalue reference".
[5] A named object that is not used again within the scope in which it is
    defined is also an expiring value.  It can be treated in the same manner as
    an unnamed temporary.
[6] Note that the std::move solution also relies on type modifiers, and also to
    a certain extent on the programmer "getting it right".  That is, the
    optimization depends on a "steal" constructor and "steal" assignment
    operator (taking an && operand) being implemented in each class.
[7] The plain object type T is mentioned, because its use can still be
    advantageous for small data-only (POD) objects. 
