#include "composite_shape.h"
#include <iomanip>
#include <algorithm>
#include <limits>

void CompositeShape::addShape(std::unique_ptr<Shape> shape) {
    shapes_.push_back(std::move(shape));
}

double CompositeShape::getArea() const {
    double area = 0.0;
    for (const auto& shape : shapes_) {
        area += shape->getArea();
    }
    return area;
}

Point CompositeShape::getCenter() const {
    if (shapes_.empty()) {
        return Point(0.0, 0.0);
    }

    Point minPt, maxPt;
    getBoundingBox(minPt, maxPt);

    return Point((minPt.x + maxPt.x) / 2.0, (minPt.y + maxPt.y) / 2.0);
}

void CompositeShape::move(double dx, double dy) {
    for (auto& shape : shapes_) {
        shape->move(dx, dy);
    }
}

void CompositeShape::scale(double coefficient) {
    if (shapes_.empty()) return;

    Point center = getCenter();

    for (auto& shape : shapes_) {
        Point shapeCenter = shape->getCenter();
        
        double dx = shapeCenter.x - center.x;
        double dy = shapeCenter.y - center.y;

        shape->move(dx * (coefficient - 1), dy * (coefficient - 1));
        shape->scale(coefficient);
    }
}

std::string CompositeShape::getName() const {
    return "COMPOSITE";
}

void CompositeShape::print(std::ostream& os) const {
    os << std::fixed << std::setprecision(2);
    Point center = getCenter();
    os << "[" << getName() << ", (" << center.x << ", " << center.y << "), " << getArea() << ":" << std::endl;
    
    for (size_t i = 0; i < shapes_.size(); ++i) {
        os << " ";
        shapes_[i]->print(os);
        if (i < shapes_.size() - 1) {
            os << ",";
        }
        os << std::endl;
    }
    
    os << "]";
}

void CompositeShape::getBoundingBox(Point& min, Point& max) const {
    if (shapes_.empty()) {
        min = Point(0, 0);
        max = Point(0, 0);
        return;
    }

    double minX = std::numeric_limits<double>::max();
    double minY = std::numeric_limits<double>::max();
    double maxX = std::numeric_limits<double>::lowest();
    double maxY = std::numeric_limits<double>::lowest();

    for (const auto& shape : shapes_) {
        Point sMin, sMax;
        shape->getBoundingBox(sMin, sMax);
        minX = std::min(minX, sMin.x);
        minY = std::min(minY, sMin.y);
        maxX = std::max(maxX, sMax.x);
        maxY = std::max(maxY, sMax.y);
    }

    min = Point(minX, minY);
    max = Point(maxX, maxY);
}
