#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "shape.h"

class Rectangle : public Shape {
private:
    Point Left;
    Point topRight;

public:
    Rectangle(Point bl, Point tr);

    double getArea() const override;
    Point getCenter() const override;
    void move(double dx, double dy) override;
    void scale(double coefficient) override;
    const char* getName() const override;
    void print() const override;

    Point getLeft() const { return Left; }
    Point getTopRight() const { return topRight; }
};

#endif// Line endings fixed
