#include "CompositeShape.h"

#include <algorithm>
#include <utility>

void CompositeShape::add(std::unique_ptr<Shape> shape)
{
    if (shape)
    {
        shapes.push_back(std::move(shape));
    }
}

double CompositeShape::getArea() const
{
    double area = 0.0;

    for (const auto& shape : shapes)
    {
        area += shape->getArea();
    }

    return area;
}

Point CompositeShape::getCenter() const
{
    if (shapes.empty())
    {
        return { 0.0, 0.0 };
    }

    return {
        (getMinX() + getMaxX()) / 2.0,
        (getMinY() + getMaxY()) / 2.0
    };
}

void CompositeShape::move(double dx, double dy)
{
    for (auto& shape : shapes)
    {
        shape->move(dx, dy);
    }
}

void CompositeShape::scale(double factor)
{
    Point compositeCenter = getCenter();

    for (auto& shape : shapes)
    {
        Point oldCenter = shape->getCenter();

        double newCenterX =
            compositeCenter.x +
            (oldCenter.x - compositeCenter.x) * factor;

        double newCenterY =
            compositeCenter.y +
            (oldCenter.y - compositeCenter.y) * factor;

        shape->move(
            newCenterX - oldCenter.x,
            newCenterY - oldCenter.y
        );

        shape->scale(factor);
    }
}

const char* CompositeShape::getName() const
{
    return "COMPOSITE";
}

double CompositeShape::getMinX() const
{
    if (shapes.empty())
    {
        return 0.0;
    }

    double minX = shapes.front()->getMinX();

    for (const auto& shape : shapes)
    {
        minX = std::min(minX, shape->getMinX());
    }

    return minX;
}

double CompositeShape::getMaxX() const
{
    if (shapes.empty())
    {
        return 0.0;
    }

    double maxX = shapes.front()->getMaxX();

    for (const auto& shape : shapes)
    {
        maxX = std::max(maxX, shape->getMaxX());
    }

    return maxX;
}

double CompositeShape::getMinY() const
{
    if (shapes.empty())
    {
        return 0.0;
    }

    double minY = shapes.front()->getMinY();

    for (const auto& shape : shapes)
    {
        minY = std::min(minY, shape->getMinY());
    }

    return minY;
}

double CompositeShape::getMaxY() const
{
    if (shapes.empty())
    {
        return 0.0;
    }

    double maxY = shapes.front()->getMaxY();

    for (const auto& shape : shapes)
    {
        maxY = std::max(maxY, shape->getMaxY());
    }

    return maxY;
}

const std::vector<std::unique_ptr<Shape>>&
CompositeShape::getShapes() const
{
    return shapes;
}