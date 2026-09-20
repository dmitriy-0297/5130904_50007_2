#ifndef COMPOSITE_SHAPE_H
#define COMPOSITE_SHAPE_H

#include "Shape.h"

#include <memory>
#include <vector>

class CompositeShape : public Shape
{
private:
    std::vector<std::unique_ptr<Shape>> shapes;

public:
    void add(std::unique_ptr<Shape> shape);

    double getArea() const override;
    Point getCenter() const override;

    void move(double dx, double dy) override;
    void scale(double factor) override;

    const char* getName() const override;

    double getMinX() const override;
    double getMaxX() const override;
    double getMinY() const override;
    double getMaxY() const override;

    const std::vector<std::unique_ptr<Shape>>& getShapes() const;
};

#endif