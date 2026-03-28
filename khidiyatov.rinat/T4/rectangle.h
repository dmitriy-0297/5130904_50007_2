#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "shape.h"

class Rectangle : public Shape {
public:
    Rectangle(const Point& bottomLeft, const Point& topRight);

    double getArea() const override;
    Point getCenter() const override;
    void move(double dx, double dy) override;
    void scale(double factor) override;
    std::string getName() const override;
    void print(std::ostream& os, bool wrap) const override;
    std::pair<Point, Point> getBounds() const override;

private:
    Point bottomLeft_;
    Point topRight_;
};

#endif 

