#ifndef ISOSCELES_TRAPEZOID_H
#define ISOSCELES_TRAPEZOID_H

#include "shape.h"

class IsoscelesTrapezoid : public Shape {
public:
    IsoscelesTrapezoid(Point bottomLeft, double bBottom, double bTop, double height);

    double getArea() const override;
    Point getCenter() const override;
    void move(double dx, double dy) override;
    void scale(double factor) override;
    std::string getName() const override;
    std::unique_ptr<Shape> clone() const override;

private:
    Point center_;
    double bBottom_, bTop_, height_;
};

#endif