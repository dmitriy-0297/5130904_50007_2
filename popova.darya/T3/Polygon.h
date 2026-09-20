#ifndef POLYGON_H
#define POLYGON_H

#include <algorithm>
#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

struct Point
{
    int x;
    int y;
};

struct Polygon
{
    std::vector<Point> points;
};

bool operator==(const Point& lhs, const Point& rhs);
bool operator==(const Polygon& lhs, const Polygon& rhs);

double getArea(const Polygon& polygon);
bool parsePolygon(const std::string& line, Polygon& polygon);
bool samePolygon(const Polygon& lhs, const Polygon& rhs);

struct Area
{
    double operator()(const Polygon& polygon) const
    {
        return getArea(polygon);
    }
};

struct Vertexes
{
    std::size_t operator()(const Polygon& polygon) const
    {
        return polygon.points.size();
    }
};

struct Same
{
    const Polygon& target;

    bool operator()(const Polygon& polygon) const
    {
        return samePolygon(polygon, target);
    }
};

struct VertexCount
{
    std::size_t target;

    bool operator()(const Polygon& polygon) const
    {
        return polygon.points.size() == target;
    }
};

void processCommand(const std::string& command,
    std::vector<Polygon>& polygons);

#endif