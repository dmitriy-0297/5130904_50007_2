#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <iomanip>
#include <cmath>
#include <iterator>
#include <sstream>

struct Point
{
  int x;
  int y;
};

struct Polygon
{
  std::vector<Point> points;
};

using PolygonContainer = std::vector<Polygon>;

const size_t MIN_VERTEX_COUNT = 3;
const double AREA_DIVISOR = 2.0;

bool operator==(const Point& lhs, const Point& rhs)
{
  return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

bool operator==(const Polygon& lhs, const Polygon& rhs)
{
  if (lhs.points.size() != rhs.points.size())
  {
    return false;
  }
  return std::equal(lhs.points.begin(), lhs.points.end(), rhs.points.begin());
}

std::istream& operator>>(std::istream& in, Point& point)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  char openParen = '\0';
  char semicolon = '\0';
  char closeParen = '\0';
  int tempX = 0;
  int tempY = 0;
  in >> openParen >> tempX >> semicolon >> tempY >> closeParen;
  if (in && (openParen == '(') && (semicolon == ';') && (closeParen == ')'))
  {
    point.x = tempX;
    point.y = tempY;
  }
  else
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& operator>>(std::istream& in, Polygon& polygon)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  size_t count = 0;
  if (!(in >> count) || (count < MIN_VERTEX_COUNT))
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  Polygon tempPolygon;
  std::generate_n(std::back_inserter(tempPolygon.points), count, [&in]()
  {
    Point point{};
    in >> point;
    return point;
  });
  if (in)
  {
    polygon = std::move(tempPolygon);
  }
  return in;
}

double calculatePolygonArea(const Polygon& polygon)
{
  if (polygon.points.size() < MIN_VERTEX_COUNT)
  {
    return 0.0;
  }
  size_t numPoints = polygon.points.size();
  std::vector<size_t> indices(numPoints);
  std::iota(indices.begin(), indices.end(), 0);
  double area = std::accumulate(indices.begin(), indices.end(), 0.0,
    [&polygon, numPoints](double sum, size_t i)
    {
      const Point& current = polygon.points[i];
      const Point& next = polygon.points[(i + 1) % numPoints];
      double crossProduct = static_cast<double>(current.x) * next.y - static_cast<double>(next.x) * current.y;
      return sum + crossProduct;
    });
  return std::abs(area) / AREA_DIVISOR;
}

void executeArea(std::istream& in, const PolygonContainer& polygons)
{
  std::string argument;
  if (!(in >> argument))
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }
  std::string trailing;
  if (in >> trailing)
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }
  double totalArea = 0.0;
  if (argument == "ODD")
  {
    totalArea = std::accumulate(polygons.begin(), polygons.end(), 0.0,
      [](double sum, const Polygon& poly)
      {
        return sum + ((poly.points.size() % 2 != 0) ? calculatePolygonArea(poly) : 0.0);
      });
  }
  else if (argument == "EVEN")
  {
    totalArea = std::accumulate(polygons.begin(), polygons.end(), 0.0,
      [](double sum, const Polygon& poly)
      {
        return sum + ((poly.points.size() % 2 == 0) ? calculatePolygonArea(poly) : 0.0);
      });
  }
  else if (argument == "MEAN")
  {
    if (polygons.empty())
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }
    double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
      [](double acc, const Polygon& poly)
      {
        return acc + calculatePolygonArea(poly);
      });
    totalArea = sum / static_cast<double>(polygons.size());
  }
  else
  {
    try
    {
      size_t vertexes = std::stoul(argument);
      if (vertexes < MIN_VERTEX_COUNT)
      {
        std::cout << "<INVALID COMMAND>\n";
        return;
      }
      totalArea = std::accumulate(polygons.begin(), polygons.end(), 0.0,
        [vertexes](double sum, const Polygon& poly)
        {
          return sum + ((poly.points.size() == vertexes) ? calculatePolygonArea(poly) : 0.0);
        });
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }
  }
  std::cout << std::fixed << std::setprecision(1) << totalArea << "\n";
}

void executeMax(std::istream& in, const PolygonContainer& polygons)
{
  std::string argument;
  if (!(in >> argument) || polygons.empty())
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }
  std::string trailing;
  if (in >> trailing)
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }
  if (argument == "AREA")
  {
    auto maxIt = std::max_element(polygons.begin(), polygons.end(),
      [](const Polygon& lhs, const Polygon& rhs)
      {
        return calculatePolygonArea(lhs) < calculatePolygonArea(rhs);
      });
    std::cout << std::fixed << std::setprecision(1) << calculatePolygonArea(*maxIt) << "\n";
  }
  else if (argument == "VERTEXES")
  {
    auto maxIt = std::max_element(polygons.begin(), polygons.end(),
      [](const Polygon& lhs, const Polygon& rhs)
      {
        return lhs.points.size() < rhs.points.size();
      });
    std::cout << maxIt->points.size() << "\n";
  }
  else
  {
    std::cout << "<INVALID COMMAND>\n";
  }
}

void executeMin(std::istream& in, const PolygonContainer& polygons)
{
  std::string argument;
  if (!(in >> argument) || polygons.empty())
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }
  std::string trailing;
  if (in >> trailing)
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }
  if (argument == "AREA")
  {
    auto minIt = std::min_element(polygons.begin(), polygons.end(),
      [](const Polygon& lhs, const Polygon& rhs)
      {
        return calculatePolygonArea(lhs) < calculatePolygonArea(rhs);
      });
    std::cout << std::fixed << std::setprecision(1) << calculatePolygonArea(*minIt) << "\n";
  }
  else if (argument == "VERTEXES")
  {
    auto minIt = std::min_element(polygons.begin(), polygons.end(),
      [](const Polygon& lhs, const Polygon& rhs)
      {
        return lhs.points.size() < rhs.points.size();
      });
    std::cout << minIt->points.size() << "\n";
  }
  else
  {
    std::cout << "<INVALID COMMAND>\n";
  }
}

void executeCount(std::istream& in, const PolygonContainer& polygons)
{
  std::string argument;
  if (!(in >> argument))
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }
  std::string trailing;
  if (in >> trailing)
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }
  long long count = 0;
  if (argument == "ODD")
  {
    count = std::count_if(polygons.begin(), polygons.end(),
      [](const Polygon& poly)
      {
        return poly.points.size() % 2 != 0;
      });
  }
  else if (argument == "EVEN")
  {
    count = std::count_if(polygons.begin(), polygons.end(),
      [](const Polygon& poly)
      {
        return poly.points.size() % 2 == 0;
      });
  }
  else
  {
    try
    {
      size_t vertexes = std::stoul(argument);
      if (vertexes < MIN_VERTEX_COUNT)
      {
        std::cout << "<INVALID COMMAND>\n";
        return;
      }
      count = std::count_if(polygons.begin(), polygons.end(),
        [vertexes](const Polygon& poly)
        {
          return poly.points.size() == vertexes;
        });
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }
  }
  std::cout << count << "\n";
}

void executeLessArea(std::istream& in, const PolygonContainer& polygons)
{
  Polygon targetPolygon;
  if (!(in >> targetPolygon))
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }
  std::string trailing;
  if (in >> trailing)
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }
  double targetArea = calculatePolygonArea(targetPolygon);
  long long count = std::count_if(polygons.begin(), polygons.end(),
    [targetArea](const Polygon& poly)
    {
      return calculatePolygonArea(poly) < targetArea;
    });
  std::cout << count << "\n";
}

void executeMaxSeq(std::istream& in, const PolygonContainer& polygons)
{
  Polygon targetPolygon;
  if (!(in >> targetPolygon))
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }
  std::string trailing;
  if (in >> trailing)
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }
  struct State
  {
    size_t maxSeq;
    size_t currentSeq;
    Polygon target;
  };
  State initialState{ 0, 0, targetPolygon };
  State finalState = std::accumulate(polygons.begin(), polygons.end(), initialState,
    [](State acc, const Polygon& poly)
    {
      if (poly == acc.target)
      {
        acc.currentSeq++;
        acc.maxSeq = std::max(acc.maxSeq, acc.currentSeq);
      }
      else
      {
        acc.currentSeq = 0;
      }
      return acc;
    });
  std::cout << finalState.maxSeq << "\n";
}

class PolygonManager
{
public:
  explicit PolygonManager(PolygonContainer polygons):
    polygons_(std::move(polygons))
  {}

  void processCommands(std::istream& in) const
  {
    std::string line;
    while (std::getline(in, line))
    {
      if (line.empty())
      {
        continue;
      }
      std::stringstream ss(line);
      std::string mainCommand;
      if (!(ss >> mainCommand))
      {
        continue;
      }

      if (mainCommand == "AREA")
      {
        executeArea(ss, polygons_);
      }
      else if (mainCommand == "MAX")
      {
        executeMax(ss, polygons_);
      }
      else if (mainCommand == "MIN")
      {
        executeMin(ss, polygons_);
      }
      else if (mainCommand == "COUNT")
      {
        executeCount(ss, polygons_);
      }
      else if (mainCommand == "LESSAREA")
      {
        executeLessArea(ss, polygons_);
      }
      else if (mainCommand == "MAXSEQ")
      {
        executeMaxSeq(ss, polygons_);
      }
      else
      {
        std::cout << "<INVALID COMMAND>\n";
      }
    }
  }

private:
  PolygonContainer polygons_;
};

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    std::cerr << "Error: Missing filename argument.\n";
    return 1;
  }
  std::ifstream inputFile(argv[1]);
  if (!inputFile.is_open())
  {
    std::cerr << "Error: Could not open file.\n";
    return 1;
  }
  PolygonContainer polygons;
  while (inputFile)
  {
    Polygon polygon;
    if (inputFile >> polygon)
    {
      polygons.push_back(polygon);
    }
    else if (!inputFile.eof())
    {
      inputFile.clear();
      std::string skip;
      std::getline(inputFile, skip);
    }
  }
  inputFile.close();

  PolygonManager manager(polygons);
  manager.processCommands(std::cin);

  return 0;
}
