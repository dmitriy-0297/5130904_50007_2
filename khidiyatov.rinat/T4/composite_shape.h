#ifndef COMPOSITE_SHAPE_H
#define COMPOSITE_SHAPE_H

#include "shape.h"
#include <vector>
#include <memory>

class CompositeShape : public Shape {
public:
    CompositeShape();
    explicit CompositeShape(std::vector<std::unique_ptr<Shape>> shapes);

    double getArea() const override;
    Point getCenter() const override;
    void move(double dx, double dy) override;
    void scale(double factor) override;
    std::string getName() const override;
    void print(std::ostream& os, bool wrap) const override;
    std::pair<Point, Point> getBounds() const override;

    void addShape(std::unique_ptr<Shape> shape);

private:
    std::vector<std::unique_ptr<Shape>> shapes_;
};

#endif

