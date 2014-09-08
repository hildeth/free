// Draw a circle (how hard can it be?)
//
// As usual, the hard part is describing precisely the problem to be solved.
// Here are my assumptions:
//  1. Want border line to be "continuous", but not "thick".
//  2. Want error to be minimized (good approximation).
//
// "Continuous" means each pixel has at least two neighbors, either TBLR or diagonally.
// "Not too thick" means each pixel has at most two neighbors.
// e.g. if the continuous line is marked as "o", then the pixels marked "x" are not allowed.
// x x x
// o o x
// x x o
//
// Math is your friend.
// Since a function is defined as having a unique range value for each member of the domain, 
// we'll get the required minimal thickness (over an interval) if we express the lit pixels
// as a function.
//
// Since we know the circle is symmetric about the main diagonals, we can break there.  We'll
// express the arcs intersecting the y axis as (two distinct) functions of x, and those
// intersecting the x axis as functions of y.
//
#include <stdio.h>

int squared_error(int x, int y, int radius)
{
  int result = x * x + y * y - radius * radius;
  if (result < 0)
    return -result;
  return result;
}

// We approximate the top of the circle by a straight line :-}
// But adjust the y component downward if the error for that pixel is smaller than for the
// tangent.
void print_top_arc(int radius)
{
  int y = radius;
  // We go from x == 0 up to x >= radius / sqrt(2).
  for (int x = 0; 2 * x * x <= radius * radius; ++x)
  {
    // Leave y the same as it was, unless the error for y-1 is less.
    if (squared_error(x, y-1, radius) < squared_error(x, y, radius))
      --y;
    printf("(%d,%d)\n", x, y);
  }
}

void print_circle(int radius)
{
  print_top_arc(radius);
//  print_bottom_arc(radius);
//  print_left_arc(radius);
//  print_right_arc(radius);
}

int main()
{
  print_circle(12);
  return 0;
}
