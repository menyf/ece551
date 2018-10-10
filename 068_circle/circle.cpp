#include "circle.h"
void Circle::move(double dx, double dy) {
  center.move(dx, dy);
}

double Circle::intersectionArea(const Circle & otherCircle) {
  // double ans = 0;
  // double dist = center.distanceFrom(otherCircle.center);
  // double d = dist / 2;

  // double r = radius;
  // double theta = acos(d / r) * 2;
  // ans += theta * r - r * r * sin(theta * 0.5) * cos(theta * 0.5);

  // r = otherCircle.radius;
  // double alpha = acos(d / r) * 2;
  // ans += alpha * r - r * r * sin(alpha * 0.5) * cos(alpha * 0.5);

  double d = center.distanceFrom(otherCircle.center);
  double r1 = radius < otherCircle.radius ? radius : otherCircle.radius;
  double r2 = radius > otherCircle.radius ? radius : otherCircle.radius;
  if (r1 + r2 <= d)
    return 0;
  else if (r2 - r1 >= d)
    return acos(-1.0) * r1 * r1;
  double a1 = acos((r1 * r1 + d * d - r2 * r2) / (2.0 * r1 * d));
  double a2 = acos((r2 * r2 + d * d - r1 * r1) / (2.0 * r2 * d));

  return a1 * r1 * r1 + a2 * r2 * r2 - r1 * d * sin(a1);
}
