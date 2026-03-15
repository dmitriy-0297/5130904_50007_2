#ifndef POINT_H
#define POINT_H

class Point {
public:
    double x_;
    double y_;

    Point()
        : x_(0.0)
        , y_(0.0)
    {}

    Point(double xVal, double yVal)
        : x_(xVal)
        , y_(yVal)
    {}
};

#endif
