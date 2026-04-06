#include "Circle.h"
#include <cmath>
#include <stdexcept>

Circle::Circle(const Point& center, double radius):
  center_(center),
  radius_(radius)
{
  if (radius_ <= 0.0)
  {
    throw std::invalid_argument("Radius must be positive");
  }
}

double Circle::getArea() const
{
  return M_PI * radius_ * radius_;
}

Point Circle::getCenter() const
{
  return center_;
}

void Circle::move(double dx, double dy)
{
  center_.x_ += dx;
  center_.y_ += dy;
}

void Circle::move(const Point& newCenter)
{
  center_ = newCenter;
}

void Circle::scale(double factor)
{
  if (factor <= 0.0)
  {
    throw std::invalid_argument("Scale factor must be positive");
  }
  radius_ *= factor;
}

std::string Circle::getName() const
{
  return "CIRCLE";
}