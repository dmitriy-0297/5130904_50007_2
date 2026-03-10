#ifndef COMPOSITE_SHAPE_H
#define COMPOSITE_SHAPE_H

#include "shape.h"
#include <vector>
#include <memory>

class CompositeShape : public Shape {
private:
    std::vector<std::unique_ptr<Shape>> shapes;
    
    Point getBoundingBoxMin() const;
    Point getBoundingBoxMax() const;
    
public:
    CompositeShape();
    
    void addShape(std::unique_ptr<Shape> shape);
    
    double getArea() const override;
    Point getCenter() const override;
    void move(double dx, double dy) override;
    void scale(double coefficient) override;
    const char* getName() const override;
    void print() const override;
    
    size_t getShapeCount() const { return shapes.size(); }
};

#endif