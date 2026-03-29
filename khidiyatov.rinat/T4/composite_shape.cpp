#include "composite_shape.h"
#include <iostream>
#include <limits>
#include <iomanip>

CompositeShape::CompositeShape() : shapes_() {}

CompositeShape::CompositeShape(std::vector<std::unique_ptr<Shape>> shapes) :
    shapes_(std::move(shapes)) {
}

void CompositeShape::addShape(std::unique_ptr<Shape> shape) {
    shapes_.push_back(std::move(shape));
}

double CompositeShape::getArea() const {
    double totalArea = 0.0;
    for (const auto& shape : shapes_) {
        totalArea += shape->getArea();
    }
    return totalArea;
}

std::pair<Point, Point> CompositeShape::getBounds() const {
    double minX = std::numeric_limits<double>::max();
    double minY = std::numeric_limits<double>::max();
    double maxX = std::numeric_limits<double>::lowest();
    double maxY = std::numeric_limits<double>::lowest();

    for (const auto& shape : shapes_) {
        std::pair<Point, Point> bounds = shape->getBounds();
        if (bounds.first.x_ < minX) {
            minX = bounds.first.x_;
        }
        if (bounds.first.y_ < minY) {
            minY = bounds.first.y_;
        }
        if (bounds.second.x_ > maxX) {
            maxX = bounds.second.x_;
        }
        if (bounds.second.y_ > maxY) {
            maxY = bounds.second.y_;
        }
    }

    return std::make_pair(Point(minX, minY), Point(maxX, maxY));
}

Point CompositeShape::getCenter() const {
    std::pair<Point, Point> bounds = getBounds();
    double centerX = (bounds.first.x_ + bounds.second.x_) / 2.0;
    double centerY = (bounds.first.y_ + bounds.second.y_) / 2.0;
    return Point(centerX, centerY);
}

void CompositeShape::move(double dx, double dy) {
    for (auto& shape : shapes_) {
        shape->move(dx, dy);
    }
}

void CompositeShape::scale(double factor) {
    Point center = getCenter();
    for (auto& shape : shapes_) {
        Point shapeCenter = shape->getCenter();
        double newX = center.x_ + (shapeCenter.x_ - center.x_) * factor;
        double newY = center.y_ + (shapeCenter.y_ - center.y_) * factor;
        double dx = newX - shapeCenter.x_;
        double dy = newY - shapeCenter.y_;
        shape->move(dx, dy);
        shape->scale(factor);
    }
}

std::string CompositeShape::getName() const {
    return "COMPOSITE";
}

void CompositeShape::print(std::ostream& os, bool) const {
    Point center = getCenter();
    double area = getArea();
    os << std::fixed << std::setprecision(2);
    os << "[" << getName() << ", (" << center.x_ << ", " << center.y_
        << "), " << area << ":" << std::endl;

    for (size_t i = 0; i < shapes_.size(); ++i) {
        os << "  ";
        shapes_[i]->print(os, false);
        os << "," << std::endl;
    }
    os << "]" << std::endl;
}
