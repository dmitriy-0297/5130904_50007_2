#ifndef TRAPEZ_H
#define TRAPEZ_H

#include "shape.h"

class Trapezoid : public Shape {
private:
    Point Left;
    double Base;
    double topBase;
    double height;

public:
    Trapezoid(Point bl, double Base, double topBase, double height);

    double getArea() const override;
    Point getCenter() const override;
    void move(double dx, double dy) override;
    void scale(double coefficient) override;
    const char* getName() const override;
    void print() const override;
};

#endif// Line endings fixed
