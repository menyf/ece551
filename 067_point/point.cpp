#include "point.h"
void Point::move(double dx, double dy) {
  x += dx;
  y += dy;
}

double Point::distanceFrom(const Point & p) {
  double diff_x = p.x - x;
  double diff_y = p.y - y;
  return sqrt(diff_x * diff_x + diff_y * diff_y);
}
