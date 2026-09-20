#ifndef COMPOSITESHAPE_H
#define COMPOSITESHAPE_H
#include"Shape.h"
#include"Point.h"
#include<vector>
#include<memory>
class CompositeShape :public Shape {
private:
    std::vector<std::unique_ptr<Shape>> shapes_;
public:
    CompositeShape() = default;
    void addShape(std::unique_ptr<Shape> shape);
    double getArea() const override;
    Point getCenter() const override;
    void move(double dx, double dy) override;
    void scale(double factor) override;
    std::string getName() const override;
    double getMinX() const override;
    double getMinY() const override;
    double getMaxX() const override;
    double getMaxY() const override;
    size_t getShapeCount() const { return shapes_.size(); }
    const Shape* getShape(size_t index) const {
        return shapes_[index].get();
    }
};
#endif
