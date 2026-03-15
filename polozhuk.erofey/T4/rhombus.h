
#ifndef INC_5130904_50007_2_RHOMBUS_H
#define INC_5130904_50007_2_RHOMBUS_H
#include "shape.h"
#include "point.h"
#include <string>

class Rhombus : public Shape{
    public:
    Rhombus(Point cross, double high, double weight);
    Rhombus(double centerX, double centerY, double high, double weight)
    : cross_(centerX, centerY), high_(high), weight_(weight) {}
    double getArea() const override;
    void move(double dx, double dy) override;
    void scale(double k) override;
    std::string getName() const override;
    Point getCenter() const override;
    Point getTop() const;
    Point getBottom() const;
    Point getLeft() const;
    Point getRight() const;
    double getHigh() const { return high_; }
    double getWeight() const { return weight_; }
    void print() const override;
    private:
    Point cross_;
    double high_;
    double weight_;
};
#endif //INC_5130904_50007_2_RHOMBUS_H




