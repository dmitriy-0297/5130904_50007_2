#ifndef POLYGON_MANAGER_H
#define POLYGON_MANAGER_H

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <functional>
#include <numeric>
#include <iomanip>

struct Point
{
    int x;
    int y;
};

struct Polygon
{
    std::vector<Point> points;
};

class PolygonManager
{
public:
    explicit PolygonManager(const std::vector<Polygon>& polygons);
    void processCommands() const;

private:
    std::vector<Polygon> polygons_;
    double calculateArea(const Polygon& polygon) const;
    bool comparePolygons(const Polygon& p1, const Polygon& p2) const;
    void executeArea(const std::string& param) const;
    void executeMax(const std::string& param) const;
    void executeMin(const std::string& param) const;
    void executeCount(const std::string& param) const;
    void executeLessArea() const;
    void executeMaxSeq() const;
};

#endif