#ifndef TRAPEZOID_H
#define TRAPEZOID_H

#include "Shape.h"

class Trapezoid : public Shape
{
private:
    Point leftBottom;
    double bottomBase;
    double topBase;
    double height;

public:
    Trapezoid(Point leftBottom,
        double bottomBase,
        double topBase,
        double height);

    double getArea() const override;
    Point getCenter() const override;

    void move(double dx, double dy) override;
    void scale(double factor) override;

    const char* getName() const override;

    double getMinX() const override;
    double getMaxX() const override;
    double getMinY() const override;
    double getMaxY() const override;
};

#endif