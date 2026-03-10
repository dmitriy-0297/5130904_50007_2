#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "shape.h"

class Ellipse : public Shape {
private:
    Point center_;
    double radiusX_;
    double radiusY_;

public:
    Ellipse(const Point& center, double radiusX, double radiusY);

    double getArea() const override;
    Point getCenter() const override;
    void move(double dx, double dy) override;
    void scale(double coefficient) override;
    std::string getName() const override;
    void print(std::ostream& os) const override;
    void getBoundingBox(Point& min, Point& max) const override;
};

#endif
