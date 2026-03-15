#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "shape.h"

class Rectangle : public Shape
{
public:
    Rectangle(Point bl, Point tr);

    double getArea() const override;
    Point getCenter() const override;
    void move(double dx, double dy) override;
    void scale(double k) override;
    const char* getName() const override;
    void print() const override;

    Point getBottomLeft() const;
    Point getTopRight() const;

private:
    Point lowLeft_;
    Point topRight_;
};

#endif
