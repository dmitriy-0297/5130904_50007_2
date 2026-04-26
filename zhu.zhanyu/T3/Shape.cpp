#include "Shape.h"
#include <fstream>
#include <sstream>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <numeric>

bool operator==(const Point& a, const Point& b)
{
  return a.x == b.x && a.y == b.y;
}

bool operator<(const Point& a, const Point& b)
{
  if (a.x != b.x)
    return a.x < b.x;
  return a.y < b.y;
}

bool are_permutations(const Polygon& a, const Polygon& b)
{
  if (a.points.size() != b.points.size())
    return false;

  std::vector<Point> sorted_a = a.points;
  std::vector<Point> sorted_b = b.points;

  std::sort(sorted_a.begin(), sorted_a.end());
  std::sort(sorted_b.begin(), sorted_b.end());

  return sorted_a == sorted_b;
}

std::vector<Polygon> read_polygons(const std::string& filename)
{
  std::vector<Polygon> result;
  std::ifstream file(filename);
  if (!file.is_open())
    return result;

  std::string line;
  while (std::getline(file, line))
  {
    if (line.empty())
      continue;

    std::istringstream iss(line);
    int n = 0;
    if (!(iss >> n) || n < 3)
      continue;

    Polygon poly;
    bool ok = true;
    for (int i = 0; i < n; ++i)
    {
      char ch;
      int x = 0, y = 0;
      if (!(iss >> ch) || ch != '(')
      {
        ok = false;
        break;
      }
      if (!(iss >> x))
      {
        ok = false;
        break;
      }
      if (!(iss >> ch) || ch != ';')
      {
        ok = false;
        break;
      }
      if (!(iss >> y))
      {
        ok = false;
        break;
      }
      if (!(iss >> ch) || ch != ')')
      {
        ok = false;
        break;
      }
      poly.points.push_back({x, y});
    }

    if (ok && static_cast<int>(poly.points.size()) == n)
    {
      std::string extra;
      iss >> extra;
      if (extra.empty())
        result.push_back(poly);
    }
  }

  return result;
}

double area(const Polygon& poly)
{
  int n = poly.points.size();
  if (n < 3)
    return 0.0;

  double sum = 0.0;
  for (int i = 0; i < n; ++i)
  {
    int j = (i + 1) % n;
    sum += poly.points[i].x * poly.points[j].y
         - poly.points[j].x * poly.points[i].y;
  }

  return std::abs(sum) / 2.0;
}

void process_commands(std::vector<Polygon>& polygons)
{
  std::string line;
  while (std::getline(std::cin, line))
  {
    if (line.empty())
      continue;

    std::istringstream iss(line);
    std::string cmd;
    iss >> cmd;

    if (cmd == "AREA")
    {
      std::string param;
      iss >> param;

      if (param == "EVEN")
      {
        double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
          [](double acc, const Polygon& p) {
            if (p.points.size() % 2 == 0)
              return acc + area(p);
            return acc;
          });
        std::cout << std::fixed << std::setprecision(1) << sum << "\n";
      }
      else if (param == "ODD")
      {
        double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
          [](double acc, const Polygon& p) {
            if (p.points.size() % 2 != 0)
              return acc + area(p);
            return acc;
          });
        std::cout << std::fixed << std::setprecision(1) << sum << "\n";
      }
      else if (param == "MEAN")
      {
        if (polygons.empty())
        {
          std::cout << "<INVALID COMMAND>\n";
        }
        else
        {
          double total = std::accumulate(polygons.begin(), polygons.end(), 0.0,
            [](double acc, const Polygon& p) {
              return acc + area(p);
            });
          double mean = total / polygons.size();
          std::cout << std::fixed << std::setprecision(1) << mean << "\n";
        }
      }
      else
      {
        int n = std::stoi(param);
        if (n < 3)
        {
          std::cout << "<INVALID COMMAND>\n";
        }
        else
        {
          double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
            [n](double acc, const Polygon& p) {
              if (static_cast<int>(p.points.size()) == n)
                return acc + area(p);
              return acc;
            });
          std::cout << std::fixed << std::setprecision(1) << sum << "\n";
        }
      }
    }
    else if (cmd == "MAX")
    {
      std::string param;
      iss >> param;

      if (polygons.empty())
      {
        std::cout << "<INVALID COMMAND>\n";
      }
      else if (param == "AREA")
      {
        auto it = std::max_element(polygons.begin(), polygons.end(),
          [](const Polygon& a, const Polygon& b) {
            return area(a) < area(b);
          });
        std::cout << std::fixed << std::setprecision(1) << area(*it) << "\n";
      }
      else if (param == "VERTEXES")
      {
        auto it = std::max_element(polygons.begin(), polygons.end(),
          [](const Polygon& a, const Polygon& b) {
            return a.points.size() < b.points.size();
          });
        std::cout << it->points.size() << "\n";
      }
      else
      {
        std::cout << "<INVALID COMMAND>\n";
      }
    }
    else if (cmd == "MIN")
    {
      std::string param;
      iss >> param;

      if (polygons.empty())
      {
        std::cout << "<INVALID COMMAND>\n";
      }
      else if (param == "AREA")
      {
        auto it = std::min_element(polygons.begin(), polygons.end(),
          [](const Polygon& a, const Polygon& b) {
            return area(a) < area(b);
          });
        std::cout << std::fixed << std::setprecision(1) << area(*it) << "\n";
      }
      else if (param == "VERTEXES")
      {
        auto it = std::min_element(polygons.begin(), polygons.end(),
          [](const Polygon& a, const Polygon& b) {
            return a.points.size() < b.points.size();
          });
        std::cout << it->points.size() << "\n";
      }
      else
      {
        std::cout << "<INVALID COMMAND>\n";
      }
    }
    else if (cmd == "COUNT")
    {
      std::string param;
      iss >> param;

      if (param == "EVEN")
      {
        int cnt = std::count_if(polygons.begin(), polygons.end(),
          [](const Polygon& p) {
            return p.points.size() % 2 == 0;
          });
        std::cout << cnt << "\n";
      }
      else if (param == "ODD")
      {
        int cnt = std::count_if(polygons.begin(), polygons.end(),
          [](const Polygon& p) {
            return p.points.size() % 2 != 0;
          });
        std::cout << cnt << "\n";
      }
      else
      {
        int n = std::stoi(param);
        if (n < 3)
        {
          std::cout << "<INVALID COMMAND>\n";
        }
        else
        {
          int cnt = std::count_if(polygons.begin(), polygons.end(),
            [n](const Polygon& p) {
              return static_cast<int>(p.points.size()) == n;
            });
          std::cout << cnt << "\n";
        }
      }
    }
    else if (cmd == "PERMS")
    {
      Polygon target;
      int n = 0;
      iss >> n;
      if (n < 3)
      {
        std::cout << "<INVALID COMMAND>\n";
      }
      else
      {
        bool ok = true;
        std::generate_n(std::back_inserter(target.points), n,
          [&iss, &ok]() -> Point {
            Point pt = {0, 0};
            char ch;
            if (!(iss >> ch) || ch != '(') { ok = false; return pt; }
            if (!(iss >> pt.x)) { ok = false; return pt; }
            if (!(iss >> ch) || ch != ';') { ok = false; return pt; }
            if (!(iss >> pt.y)) { ok = false; return pt; }
            if (!(iss >> ch) || ch != ')') { ok = false; return pt; }
            return pt;
          });

        std::string extra;
        iss >> extra;
        if (!ok || static_cast<int>(target.points.size()) != n || !extra.empty())
        {
          std::cout << "<INVALID COMMAND>\n";
        }
        else
        {
          int cnt = std::count_if(polygons.begin(), polygons.end(),
            [&target](const Polygon& p) {
              return are_permutations(p, target);
            });
          std::cout << cnt << "\n";
        }
      }
    }
    else if (cmd == "INFRAME")
    {
      Polygon target;
      int n = 0;
      iss >> n;
      if (n < 3)
      {
        std::cout << "<INVALID COMMAND>\n";
      }
      else
      {
        bool ok = true;
        std::generate_n(std::back_inserter(target.points), n,
          [&iss, &ok]() -> Point {
            Point pt = {0, 0};
            char ch;
            if (!(iss >> ch) || ch != '(') { ok = false; return pt; }
            if (!(iss >> pt.x)) { ok = false; return pt; }
            if (!(iss >> ch) || ch != ';') { ok = false; return pt; }
            if (!(iss >> pt.y)) { ok = false; return pt; }
            if (!(iss >> ch) || ch != ')') { ok = false; return pt; }
            return pt;
          });

        std::string extra;
        iss >> extra;
        if (!ok || static_cast<int>(target.points.size()) != n || !extra.empty())
        {
          std::cout << "<INVALID COMMAND>\n";
        }
        else if (polygons.empty())
        {
          std::cout << "<INVALID COMMAND>\n";
        }
        else
        {
          auto all_points = std::accumulate(polygons.begin(), polygons.end(),
            std::vector<Point>(),
            [](std::vector<Point> acc, const Polygon& poly) {
              acc.insert(acc.end(), poly.points.begin(), poly.points.end());
              return acc;
            });

          auto x_cmp = [](const Point& a, const Point& b) { return a.x < b.x; };
          auto y_cmp = [](const Point& a, const Point& b) { return a.y < b.y; };

          int min_x = std::min_element(all_points.begin(), all_points.end(), x_cmp)->x;
          int max_x = std::max_element(all_points.begin(), all_points.end(), x_cmp)->x;
          int min_y = std::min_element(all_points.begin(), all_points.end(), y_cmp)->y;
          int max_y = std::max_element(all_points.begin(), all_points.end(), y_cmp)->y;

          bool inside = std::all_of(target.points.begin(), target.points.end(),
            [min_x, max_x, min_y, max_y](const Point& pt) {
              return pt.x >= min_x && pt.x <= max_x
                  && pt.y >= min_y && pt.y <= max_y;
            });

          if (inside)
            std::cout << "<TRUE>\n";
          else
            std::cout << "<FALSE>\n";
        }
      }
    }
    else
    {
      std::cout << "<INVALID COMMAND>\n";
    }
  }
}
