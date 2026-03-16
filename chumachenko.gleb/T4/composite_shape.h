#ifndef COMPOSITE_SHAPE_H
#define COMPOSITE_SHAPE_H

#include <vector>
#include "shape.h"

class CompositeShape : public Shape {
public:
    CompositeShape() = default;

    ~CompositeShape() override = default;
    CompositeShape(const CompositeShape& other);
    CompositeShape& operator=(const CompositeShape& other);
    CompositeShape(CompositeShape&& other) noexcept = default;
    CompositeShape& operator=(CompositeShape&& other) noexcept = default;

    void add(std::unique_ptr<Shape> shape);
    double getArea() const override;
    Point getCenter() const override;
    void move(double dx, double dy) override;
    void scale(double factor) override;
    std::string getName() const override;
    std::unique_ptr<Shape> clone() const override;

    const std::vector<std::unique_ptr<Shape>>& getShapes() const { return shapes_; }

private:
    std::vector<std::unique_ptr<Shape>> shapes_;
};

#endif

