#include "Rectangle.h"
#include <stdexcept>

Rectangle::Rectangle(const Point& bottomLeft, const Point& topRight):
  bottomLeft_(bottomLeft),
  topRight_(topRight)
{
  if (bottomLeft_.x_ >= topRight_.x_ || bottomLeft_.y_ >= topRight_.y_)
  {
    throw std::invalid_argument("Invalid rectangle coordinates");
  }
}

double Rectangle::getArea() const
{
  return (topRight_.x_ - bottomLeft_.x_) * (topRight_.y_ - bottomLeft_.y_);
}

Point Rectangle::getCenter() const
{
  return {(bottomLeft_.x_ + topRight_.x_) / 2.0, (bottomLeft_.y_ + topRight_.y_) / 2.0};
}

void Rectangle::move(double dx, double dy)
{
  bottomLeft_.x_ += dx;
  bottomLeft_.y_ += dy;
  topRight_.x_ += dx;
  topRight_.y_ += dy;
}

void Rectangle::move(const Point& newCenter)
{
  Point currentCenter = getCenter();
  move(newCenter.x_ - currentCenter.x_, newCenter.y_ - currentCenter.y_);
}

void Rectangle::scale(double factor)
{
  if (factor <= 0.0)
  {
    throw std::invalid_argument("Scale factor must be positive");
  }
  Point center = getCenter();
  double halfWidth = (topRight_.x_ - bottomLeft_.x_) * factor / 2.0;
  double halfHeight = (topRight_.y_ - bottomLeft_.y_) * factor / 2.0;

  bottomLeft_ = {center.x_ - halfWidth, center.y_ - halfHeight};
  topRight_ = {center.x_ + halfWidth, center.y_ + halfHeight};
}

std::string Rectangle::getName() const
{
  return "RECTANGLE";
}