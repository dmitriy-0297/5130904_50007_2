#ifndef INC_5130904_50007_2_RECTANGLE_H
#define INC_5130904_50007_2_RECTANGLE_H
#include "point.h"
#include <string>
#include "shape.h"
class Rectangle:  public Shape {
public:
    Rectangle(Point rh, Point ld): ld_(ld), rh_(rh) {}
    Rectangle(const Rectangle& other) : ld_(other.ld_), rh_(other.rh_) {};
    Rectangle(double x1, double y1, double x2, double y2)
    : ld_(x1, y1), rh_(x2, y2) {};
    Point getLd() const{ return ld_; };
    Point getRh() const{ return rh_; };
    double getArea() const override;
    Point getCenter() const override;
    void move(double x, double y)  override;
    void scale (double k) override;
    std::string getName() const override;
    void print () const override;
    ~Rectangle() override=default;
private:
    Point ld_;
    Point rh_;
};
#endif //INC_5130904_50007_2_RECTANGLE_H




