#ifndef INC_5130904_50007_2_COMPOSITESHAPE_H
#define INC_5130904_50007_2_COMPOSITESHAPE_H
#include "shape.h"
#include "point.h"
#include <vector>
#include <memory>
class CompositeShape: public Shape {
public:
    CompositeShape()= default;
    const Shape* getShape(size_t index) const {return shapes_[index].get();}
    void addShape(std::unique_ptr<Shape> shape);
    double getArea() const override;
    Point getCenter() const override;
    void move(double x, double y) override;
    void scale(double k) override;
    std::string getName() const override;
    size_t getSize() const {return shapes_.size();}
    void print() const override;
    ~CompositeShape() override = default;
private:
    std::vector<std::unique_ptr<Shape>> shapes_;
};
#endif





