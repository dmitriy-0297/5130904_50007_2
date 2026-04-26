#ifndef SHAPES_HPP
#define SHAPES_HPP

#include <vector>
#include <string>
#include <algorithm>

struct Point
{
  int x;
  int y;
};

struct Polygon
{
  std::vector<Point> points;
};

bool operator==(const Point& a, const Point& b);
bool operator<(const Point& a, const Point& b);
bool are_permutations(const Polygon& a, const Polygon& b);
std::vector<Polygon> read_polygons(const std::string& filename);
double area(const Polygon& poly);
void process_commands(std::vector<Polygon>& polygons);

#endif
