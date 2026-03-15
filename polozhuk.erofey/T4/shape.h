#ifndef INC_5130904_50007_2_SHAPE_H
#define INC_5130904_50007_2_SHAPE_H
#include <string>
#include "point.h"
class Shape {
public:
    virtual double getArea()  const = 0;
    virtual Point getCenter()  const = 0;
    virtual void move(double x, double y)  = 0;
    virtual void scale(double k) = 0 ;
    virtual std::string getName() const = 0;
    virtual ~Shape() = default;
    virtual void print() const {} ;
};
#endif




