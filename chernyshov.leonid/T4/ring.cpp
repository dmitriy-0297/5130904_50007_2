#include "ring.h"
#include <stdexcept>
#include <cmath>

namespace {
    const double PI = 3.14159265358979323846;
}

Ring::Ring(const Point& center, double outerRadius, double innerRadius)
    : center_(center),
      outerRadius_(outerRadius),
      innerRadius_(innerRadius) {
    if (outerRadius_ <= 0.0) {
        throw std::invalid_argument("Ring outer radius must be positive");
    }

    if (innerRadius_ <= 0.0) {
        throw std::invalid_argument("Ring inner radius must be positive");
    }

    if (innerRadius_ >= outerRadius_) {
        throw std::invalid_argument(
            "Ring inner radius must be less than outer");
    }
}

double Ring::getArea() const {
    return PI * (outerRadius_ * outerRadius_
                 - innerRadius_ * innerRadius_);
}

Point Ring::getCenter() const {
    return center_;
}

void Ring::move(double dx, double dy) {
    center_.x += dx;
    center_.y += dy;
}

void Ring::scale(double factor) {
    if (factor <= 0.0) {
        throw std::invalid_argument("Scale factor must be positive");
    }

    outerRadius_ *= factor;
    innerRadius_ *= factor;
}

std::string Ring::getName() const {
    return "RING";
}

std::unique_ptr<Shape> Ring::clone() const {
    return std::make_unique<Ring>(*this);
}
