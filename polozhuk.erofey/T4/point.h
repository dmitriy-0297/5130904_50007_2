#ifndef INC_5130904_50007_2_POINT_H
#define INC_5130904_50007_2_POINT_H
#include <iostream>
struct Point {
    Point(double x, double y) : x_(x), y_(y) {};
    double x_;
    double y_;
};
std::ostream& operator<<(std::ostream& os, const Point& p) {
    os << "(" << p.x_ << ", " << p.y_ << ")";
    return os;
}
#endif //INC_5130904_50007_2_POINT_H

