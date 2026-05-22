#ifndef POLYGON_H
#define POLYGON_H

#include <vector>
#include <iostream>
#include <string>
#include <iomanip>

struct Point
{
    int x;
    int y;
};

struct Polygon
{
    std::vector< Point > points;
};

class iofmtguard
{
public:
    iofmtguard(std::basic_ios< char >& s);
    ~iofmtguard();
private:
    std::basic_ios< char >& s_;
    std::streamsize width_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
};

bool operator==(const Point& left, const Point& right);
bool operator==(const Polygon& left, const Polygon& right);

std::istream& operator>>(std::istream& in, Point& point);
std::istream& operator>>(std::istream& in, Polygon& polygon);

double calculatePolygonArea(const Polygon& polygon);
bool isRectangle(const Polygon& polygon);

void processArea(const std::vector< Polygon >& polygons, std::istream& in);
void processMax(const std::vector< Polygon >& polygons, std::istream& in);
void processMin(const std::vector< Polygon >& polygons, std::istream& in);
void processCount(const std::vector< Polygon >& polygons, std::istream& in);
void processRects(const std::vector< Polygon >& polygons);
void processMaxSeq(const std::vector< Polygon >& polygons, std::istream& in);

void printInvalidCommand();

#endif

