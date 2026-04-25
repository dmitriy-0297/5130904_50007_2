#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <numeric>
#include <iterator>
#include <iomanip>
#include <cmath>

struct Point
{
  int x;
  int y;
};

struct Polygon
{
  std::vector< Point > points;
};

bool operator==(const Point & lhs, const Point & rhs)
{
  return lhs.x == rhs.x && lhs.y == rhs.y;
}

bool operator==(const Polygon & lhs, const Polygon & rhs)
{
  if (lhs.points.size() != rhs.points.size())
  {
    return false;
  }
  return std::equal(lhs.points.cbegin(), lhs.points.cend(), rhs.points.cbegin());
}

std::istream & operator>>(std::istream & in, Point & point)
{
  char leftParen = 0;
  char semicolon = 0;
  char rightParen = 0;
  if (!(in >> leftParen) || leftParen != '(')
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  if (!(in >> point.x))
  {
    return in;
  }
  if (!(in >> semicolon) || semicolon != ';')
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  if (!(in >> point.y))
  {
    return in;
  }
  if (!(in >> rightParen) || rightParen != ')')
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  return in;
}

std::istream & operator>>(std::istream & in, Polygon & polygon)
{
  polygon.points.clear();
  size_t count = 0;
  if (!(in >> count) || count < 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  polygon.points.reserve(count);
  for (size_t i = 0; i < count; ++i)
  {
    Point p{};
    if (!(in >> p))
    {
      return in;
    }
    polygon.points.push_back(p);
  }
  return in;
}

std::ostream & operator<<(std::ostream & out, const Point & point)
{
  out << '(' << point.x << ';' << point.y << ')';
  return out;
}

double computeArea(const Polygon & polygon)
{
  const std::vector< Point > & pts = polygon.points;
  size_t vertexCount = pts.size();
  double area = 0.0;
  for (size_t i = 0; i < vertexCount; ++i)
  {
    size_t j = (i + 1) % vertexCount;
    area += static_cast< double >(pts[i].x) * pts[j].y;
    area -= static_cast< double >(pts[j].x) * pts[i].y;
  }
  return std::abs(area) / 2.0;
}

bool isEven(const Polygon & polygon)
{
  return polygon.points.size() % 2 == 0;
}

bool isOdd(const Polygon & polygon)
{
  return polygon.points.size() % 2 != 0;
}

bool hasNVertexes(const Polygon & polygon, size_t n)
{
  return polygon.points.size() == n;
}

bool hasRightAngle(const Point & prev, const Point & curr, const Point & next)
{
  int edgeX1 = curr.x - prev.x;
  int edgeY1 = curr.y - prev.y;
  int edgeX2 = next.x - curr.x;
  int edgeY2 = next.y - curr.y;
  return (edgeX1 * edgeX2 + edgeY1 * edgeY2) == 0;
}

bool polygonHasRightAngle(const Polygon & polygon)
{
  const std::vector< Point > & pts = polygon.points;
  size_t vertexCount = pts.size();
  for (size_t i = 0; i < vertexCount; ++i)
  {
    const Point & prev = pts[(i + vertexCount - 1) % vertexCount];
    const Point & curr = pts[i];
    const Point & next = pts[(i + 1) % vertexCount];
    if (hasRightAngle(prev, curr, next))
    {
      return true;
    }
  }
  return false;
}

void cmdArea(const std::vector< Polygon > & polygons, std::istream & in, std::ostream & out)
{
  std::string param;
  if (!(in >> param))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  out << std::fixed << std::setprecision(1);
  if (param == "EVEN")
  {
    std::vector< double > areas;
    std::transform(
      polygons.cbegin(), polygons.cend(),
      std::back_inserter(areas),
      [](const Polygon & p) { return isEven(p) ? computeArea(p) : 0.0; }
    );
    out << std::accumulate(areas.cbegin(), areas.cend(), 0.0) << "\n";
  }
  else if (param == "ODD")
  {
    std::vector< double > areas;
    std::transform(
      polygons.cbegin(), polygons.cend(),
      std::back_inserter(areas),
      [](const Polygon & p) { return isOdd(p) ? computeArea(p) : 0.0; }
    );
    out << std::accumulate(areas.cbegin(), areas.cend(), 0.0) << "\n";
  }
  else if (param == "MEAN")
  {
    if (polygons.empty())
    {
      out << "<INVALID COMMAND>\n";
      return;
    }
    std::vector< double > areas;
    std::transform(
      polygons.cbegin(), polygons.cend(),
      std::back_inserter(areas),
      computeArea
    );
    out << std::accumulate(areas.cbegin(), areas.cend(), 0.0) / polygons.size() << "\n";
  }
  else
  {
    bool isNumber = !param.empty() && std::all_of(param.cbegin(), param.cend(), ::isdigit);
    if (!isNumber)
    {
      out << "<INVALID COMMAND>\n";
      return;
    }
    size_t vertexCount = std::stoull(param);
    if (vertexCount < 3)
    {
      out << "<INVALID COMMAND>\n";
      return;
    }
    std::vector< double > areas;
    auto getAreaIfNVertexes = [vertexCount](const Polygon & p)
    {
      return hasNVertexes(p, vertexCount) ? computeArea(p) : 0.0;
    };
    std::transform(
      polygons.cbegin(), polygons.cend(),
      std::back_inserter(areas),
      getAreaIfNVertexes
    );
    out << std::accumulate(areas.cbegin(), areas.cend(), 0.0) << "\n";
  }
}

void cmdMax(const std::vector< Polygon > & polygons, std::istream & in, std::ostream & out)
{
  std::string param;
  if (!(in >> param))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  if (polygons.empty())
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  if (param == "AREA")
  {
    auto it = std::max_element(
      polygons.cbegin(), polygons.cend(),
      std::bind(
        std::less< double >(),
        std::bind(computeArea, std::placeholders::_1),
        std::bind(computeArea, std::placeholders::_2)
      )
    );
    out << std::fixed << std::setprecision(1) << computeArea(*it) << "\n";
  }
  else if (param == "VERTEXES")
  {
    auto it = std::max_element(
      polygons.cbegin(), polygons.cend(),
      std::bind(
        std::less< size_t >(),
        std::bind(&std::vector< Point >::size, std::bind(&Polygon::points, std::placeholders::_1)),
        std::bind(&std::vector< Point >::size, std::bind(&Polygon::points, std::placeholders::_2))
      )
    );
    out << it->points.size() << "\n";
  }
  else
  {
    out << "<INVALID COMMAND>\n";
  }
}

void cmdMin(const std::vector< Polygon > & polygons, std::istream & in, std::ostream & out)
{
  std::string param;
  if (!(in >> param))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  if (polygons.empty())
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  if (param == "AREA")
  {
    auto it = std::min_element(
      polygons.cbegin(), polygons.cend(),
      std::bind(
        std::less< double >(),
        std::bind(computeArea, std::placeholders::_1),
        std::bind(computeArea, std::placeholders::_2)
      )
    );
    out << std::fixed << std::setprecision(1) << computeArea(*it) << "\n";
  }
  else if (param == "VERTEXES")
  {
    auto it = std::min_element(
      polygons.cbegin(), polygons.cend(),
      std::bind(
        std::less< size_t >(),
        std::bind(&std::vector< Point >::size, std::bind(&Polygon::points, std::placeholders::_1)),
        std::bind(&std::vector< Point >::size, std::bind(&Polygon::points, std::placeholders::_2))
      )
    );
    out << it->points.size() << "\n";
  }
  else
  {
    out << "<INVALID COMMAND>\n";
  }
}

void cmdCount(const std::vector< Polygon > & polygons, std::istream & in, std::ostream & out)
{
  std::string param;
  if (!(in >> param))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  if (param == "EVEN")
  {
    out << std::count_if(polygons.cbegin(), polygons.cend(), isEven) << "\n";
  }
  else if (param == "ODD")
  {
    out << std::count_if(polygons.cbegin(), polygons.cend(), isOdd) << "\n";
  }
  else
  {
    bool isNumber = !param.empty() && std::all_of(param.cbegin(), param.cend(), ::isdigit);
    if (!isNumber)
    {
      out << "<INVALID COMMAND>\n";
      return;
    }
    size_t vertexCount = std::stoull(param);
    if (vertexCount < 3)
    {
      out << "<INVALID COMMAND>\n";
      return;
    }
    out << std::count_if(
      polygons.cbegin(), polygons.cend(),
      std::bind(hasNVertexes, std::placeholders::_1, vertexCount)
    ) << "\n";
  }
}

void cmdEcho(std::vector< Polygon > & polygons, std::istream & in, std::ostream & out)
{
  Polygon target{};
  if (!(in >> target))
  {
    in.clear();
    std::string rest;
    std::getline(in, rest);
    out << "<INVALID COMMAND>\n";
    return;
  }
  std::string rest;
  if (std::getline(in, rest) && !rest.empty() && rest.find_first_not_of(' ') != std::string::npos)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  size_t addedCount = std::count(polygons.cbegin(), polygons.cend(), target);
  std::vector< Polygon > resultPolygons;
  resultPolygons.reserve(polygons.size() + addedCount);
  for (const Polygon & p : polygons)
  {
    resultPolygons.push_back(p);
    if (p == target)
    {
      resultPolygons.push_back(p);
    }
  }
  polygons = std::move(resultPolygons);
  out << addedCount << "\n";
}

void cmdRightshapes(const std::vector< Polygon > & polygons, std::ostream & out)
{
  out << std::count_if(polygons.cbegin(), polygons.cend(), polygonHasRightAngle) << "\n";
}

int main(int argc, char * argv[])
{
  if (argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << " <filename>\n";
    return 1;
  }
  std::ifstream file(argv[1]);
  if (!file.is_open())
  {
    std::cerr << "Error: cannot open file " << argv[1] << "\n";
    return 1;
  }
  std::vector< Polygon > polygons;
  std::string line;
  while (std::getline(file, line))
  {
    std::istringstream ss(line);
    Polygon polygon{};
    if (ss >> polygon)
    {
      std::string rest;
      if (!(ss >> rest))
      {
        polygons.push_back(std::move(polygon));
      }
    }
  }
  file.close();
  std::string command;
  while (std::cin >> command)
  {
    if (command == "AREA")
    {
      cmdArea(polygons, std::cin, std::cout);
    }
    else if (command == "MAX")
    {
      cmdMax(polygons, std::cin, std::cout);
    }
    else if (command == "MIN")
    {
      cmdMin(polygons, std::cin, std::cout);
    }
    else if (command == "COUNT")
    {
      cmdCount(polygons, std::cin, std::cout);
    }
    else if (command == "ECHO")
    {
      cmdEcho(polygons, std::cin, std::cout);
    }
    else if (command == "RIGHTSHAPES")
    {
      cmdRightshapes(polygons, std::cout);
    }
    else
    {
      std::string rest;
      std::getline(std::cin, rest);
      std::cout << "<INVALID COMMAND>\n";
    }
    std::cin.clear();
  }
  return 0;
}
