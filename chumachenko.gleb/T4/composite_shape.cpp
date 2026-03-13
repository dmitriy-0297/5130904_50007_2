#include "composite_shape.h"
#include <stdexcept>
#include <algorithm>
#include <limits>

CompositeShape::CompositeShape(const CompositeShape& other) {
    for (const auto& s : other.shapes_) {
        shapes_.push_back(s->clone());
    }
}

CompositeShape& CompositeShape::operator=(const CompositeShape& other) {
    if (this != &other) {
        std::vector<std::unique_ptr<Shape>> temp;
        for (const auto& s : other.shapes_) {
            temp.push_back(s->clone());
        }
        shapes_ = std::move(temp);
    }
    return *this;
}

void CompositeShape::add(std::unique_ptr<Shape> shape) {
    if (!shape) {
        throw std::invalid_argument("Cannot add null pointer as shape");
    }
    shapes_.push_back(std::move(shape));
}

double CompositeShape::getArea() const {
    double total = 0.0;
    for (const auto& s : shapes_) {
        total += s->getArea();
    }
    return total;
}

Point CompositeShape::getCenter() const {
    if (shapes_.empty()) {
        Point emptyPoint;
        emptyPoint.x = 0.0;
        emptyPoint.y = 0.0;
        return emptyPoint;
    }

    double minX = std::numeric_limits<double>::max();
    double maxX = std::numeric_limits<double>::lowest();
    double minY = minX;
    double maxY = maxX;

    for (const auto& s : shapes_) {
        Point c = s->getCenter();
        minX = std::min(minX, c.x);
        maxX = std::max(maxX, c.x);
        minY = std::min(minY, c.y);
        maxY = std::max(maxY, c.y);
    }

    Point centerPoint;
    centerPoint.x = (minX + maxX) / 2.0;
    centerPoint.y = (minY + maxY) / 2.0;
    return centerPoint;
}

void CompositeShape::move(double dx, double dy) {
    for (auto& s : shapes_) {
        s->move(dx, dy);
    }
}

void CompositeShape::scale(double factor) {
    if (factor <= 0.0) {
        throw std::invalid_argument("Scale factor must be positive");
    }
    if (shapes_.empty()) {
        return;
    }

    Point baseCenter = getCenter();
    for (auto& s : shapes_) {
        Point currentCenter = s->getCenter();
        double dx = (currentCenter.x - baseCenter.x) * (factor - 1.0);
        double dy = (currentCenter.y - baseCenter.y) * (factor - 1.0);
        s->move(dx, dy);
        s->scale(factor);
    }
}

std::string CompositeShape::getName() const {
    return "COMPOSITE";
}

std::unique_ptr<Shape> CompositeShape::clone() const {
    return std::make_unique<CompositeShape>(*this);
}


