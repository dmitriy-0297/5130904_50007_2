#ifndef POINT_H
#define POINT_H

struct Point
{
    double x;
    double y;

    Point(double xVal = 0.0, double yVal = 0.0)
        : x(xVal)
        , y(yVal)
    {
    }
};

#endif
