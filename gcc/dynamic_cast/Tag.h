// Tag.h -*- C++ -*-
// Provides a unique tag each time its constructor is called.
// Tags can only be compared for equality and inequality (so far).

#ifdef TAG_H
#error Multiple inclusion.
#else
#define TAG_H

#include <cstdlib> // For size_t.

class Tag
{
	static size_t _class_tag;
	size_t _tag;
  public:
	~Tag() {}
    Tag() : _tag(_class_tag++) {}
	Tag(const Tag& rhs) : _tag(rhs._tag) {}
	bool operator==(const Tag& other) { return _tag == other._tag; }
	bool operator!=(const Tag& other) { return ! operator==(other); }
  private:
	// Assignment not provided.
	void operator=(const Tag&);
};

#endif
