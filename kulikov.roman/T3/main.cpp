#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <iomanip>
#include <cmath>
#include <cctype>

struct Point
{
  int x, y;
};

struct Polygon
{
  std::vector<Point> points;
};

std::istream & operator>>(std::istream & in, Point & p)
{
  char c;
  if (!(in >> c) || c != '(') { 
    in.setstate(std::ios::failbit); 
    return in; 
  }
  if (!(in >> p.x)) return in;
  if (!(in >> c) || c != ';') { 
    in.setstate(std::ios::failbit); 
    return in; 
  }
  if (!(in >> p.y)) return in;
  if (!(in >> c) || c != ')') { 
    in.setstate(std::ios::failbit); 
    return in; 
  }
  return in;
}

std::istream & operator>>(std::istream & in, Polygon & poly)
{
  int n = 0;
  if (!(in >> n) || n < 3) { 
    in.setstate(std::ios::failbit); 
    return in; 
  }

  std::vector<Point> pts(n);
  for (int i = 0; i < n; ++i) {
    if (!(in >> pts[i])) {
      in.setstate(std::ios::failbit);
      return in;
    }
  }

  char c;
  while (in.get(c)) {
    if (!std::isspace(static_cast<unsigned char>(c))) {
      in.putback(c);
      break;
    }
  }

  poly.points = std::move(pts);
  return in;
}

double area(const Polygon & poly)
{
  const auto & pts = poly.points;
  int n = static_cast<int>(pts.size());
  long long sum = 0; // используем long long для избежания переполнения
  for (int i = 0; i < n; i++)
  {
    int j = (i + 1) % n;
    sum += static_cast<long long>(pts[i].x) * pts[j].y - static_cast<long long>(pts[j].x) * pts[i].y;
  }
  return std::abs(sum) / 2.0;
}

bool isSame(const Polygon & a, const Polygon & b)
{
  if (a.points.size() != b.points.size()) return false;
  int n = static_cast<int>(a.points.size());
  if (n == 0) return true;

  for (int start = 0; start < n; start++)
  {
    int dx = b.points[start].x - a.points[0].x;
    int dy = b.points[start].y - a.points[0].y;
    bool match = true;
    for (int i = 0; i < n; i++)
    {
      int bi = (start + i) % n;
      if (a.points[i].x + dx != b.points[bi].x || a.points[i].y + dy != b.points[bi].y)
      {
        match = false;
        break;
      }
    }
    if (match) return true;
  }

  for (int start = 0; start < n; start++)
  {
    int dx = b.points[start].x - a.points[0].x;
    int dy = b.points[start].y - a.points[0].y;
    bool match = true;
    for (int i = 0; i < n; i++)
    {
      int bi = (start - i + n) % n;
      if (a.points[i].x + dx != b.points[bi].x || a.points[i].y + dy != b.points[bi].y)
      {
        match = false;
        break;
      }
    }
    if (match) return true;
  }

  return false;
}

void doArea(const std::vector<Polygon> & polys, const std::string & arg)
{
  if (polys.empty() && arg == "MEAN")
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }

  if (arg == "EVEN")
  {
    double sum = 0;
    for (const auto & p : polys)
      if (p.points.size() % 2 == 0)
        sum += area(p);
    std::cout << std::fixed << std::setprecision(1) << sum << "\n";
  }
  else if (arg == "ODD")
  {
    double sum = 0;
    for (const auto & p : polys)
      if (p.points.size() % 2 != 0)
        sum += area(p);
    std::cout << std::fixed << std::setprecision(1) << sum << "\n";
  }
  else if (arg == "MEAN")
  {
    double sum = std::accumulate(polys.begin(), polys.end(), 0.0,
      [](double acc, const Polygon & p) { return acc + area(p); });
    std::cout << std::fixed << std::setprecision(1) << sum / polys.size() << "\n";
  }
  else
  {
    int n = 0;
    try { n = std::stoi(arg); } catch (...) { 
      std::cout << "<INVALID COMMAND>\n"; 
      return; 
    }
    if (n < 3) { 
      std::cout << "<INVALID COMMAND>\n"; 
      return; 
    }
    double sum = 0;
    for (const auto & p : polys)
      if (static_cast<int>(p.points.size()) == n)
        sum += area(p);
    std::cout << std::fixed << std::setprecision(1) << sum << "\n";
  }
}

void doMax(const std::vector<Polygon> & polys, const std::string & arg)
{
  if (polys.empty()) { 
    std::cout << "<INVALID COMMAND>\n"; 
    return; 
  }
  if (arg == "AREA")
  {
    double mx = area(*std::max_element(polys.begin(), polys.end(),
      [](const Polygon & a, const Polygon & b) { return area(a) < area(b); }));
    std::cout << std::fixed << std::setprecision(1) << mx << "\n";
  }
  else if (arg == "VERTEXES")
  {
    size_t mx = std::max_element(polys.begin(), polys.end(),
      [](const Polygon & a, const Polygon & b)
      { return a.points.size() < b.points.size(); })->points.size();
    std::cout << mx << "\n";
  }
  else {
    std::cout << "<INVALID COMMAND>\n";
  }
}

void doMin(const std::vector<Polygon> & polys, const std::string & arg)
{
  if (polys.empty()) { 
    std::cout << "<INVALID COMMAND>\n"; 
    return; 
  }
  if (arg == "AREA")
  {
    double mn = area(*std::min_element(polys.begin(), polys.end(),
      [](const Polygon & a, const Polygon & b) { return area(a) < area(b); }));
    std::cout << std::fixed << std::setprecision(1) << mn << "\n";
  }
  else if (arg == "VERTEXES")
  {
    size_t mn = std::min_element(polys.begin(), polys.end(),
      [](const Polygon & a, const Polygon & b)
      { return a.points.size() < b.points.size(); })->points.size();
    std::cout << mn << "\n";
  }
  else {
    std::cout << "<INVALID COMMAND>\n";
  }
}

void doCount(const std::vector<Polygon> & polys, const std::string & arg)
{
  if (arg == "EVEN")
  {
    int cnt = std::count_if(polys.begin(), polys.end(),
      [](const Polygon & p) { return p.points.size() % 2 == 0; });
    std::cout << cnt << "\n";
  }
  else if (arg == "ODD")
  {
    int cnt = std::count_if(polys.begin(), polys.end(),
      [](const Polygon & p) { return p.points.size() % 2 != 0; });
    std::cout << cnt << "\n";
  }
  else
  {
    int n = 0;
    try { n = std::stoi(arg); } catch (...) { 
      std::cout << "<INVALID COMMAND>\n"; 
      return; 
    }
    if (n < 3) { 
      std::cout << "<INVALID COMMAND>\n"; 
      return; 
    }
    int cnt = std::count_if(polys.begin(), polys.end(),
      [n](const Polygon & p) { return static_cast<int>(p.points.size()) == n; });
    std::cout << cnt << "\n";
  }
}

void doLessArea(const std::vector<Polygon> & polys, const std::string & arg)
{
  std::istringstream ss(arg);
  Polygon target;
  if (!(ss >> target)) { 
    std::cout << "<INVALID COMMAND>\n"; 
    return; 
  }
  std::ws(ss);
  if (!ss.eof()) {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }

  double targetArea = area(target);
  int cnt = std::count_if(polys.begin(), polys.end(),
    [targetArea](const Polygon & p) { return area(p) < targetArea; });
  std::cout << cnt << "\n";
}

void doSame(const std::vector<Polygon> & polys, const std::string & arg)
{
  std::istringstream ss(arg);
  Polygon target;
  if (!(ss >> target)) { 
    std::cout << "<INVALID COMMAND>\n"; 
    return; 
  }
  std::ws(ss);
  if (!ss.eof()) {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }

  int cnt = std::count_if(polys.begin(), polys.end(),
    [&target](const Polygon & p) { return isSame(p, target); });
  std::cout << cnt << "\n";
}

int main(int argc, char * argv[])
{
  if (argc < 2)
  {
    std::cerr << "Usage: " << argv[0] << " <filename>\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file)
  {
    std::cerr << "Cannot open file: " << argv[1] << "\n";
    return 1;
  }

  std::vector<Polygon> polys;
  std::string line;
  while (std::getline(file, line))
  {
    if (line.empty()) continue;

    std::istringstream ss(line);
    Polygon poly;
    if (ss >> poly) {
      std::ws(ss);
      if (ss.eof()) {
        polys.push_back(poly);
      }
    }
  }

  while (std::getline(std::cin, line))
  {
    if (line.empty()) continue;

    std::istringstream ss(line);
    std::string cmd;
    ss >> cmd;

    std::string rest;
    std::getline(ss, rest);
    if (!rest.empty() && rest[0] == ' ')
      rest = rest.substr(1);

    if (cmd == "AREA")
    {
      std::string arg;
      std::istringstream as(rest);
      if (!(as >> arg)) { 
        std::cout << "<INVALID COMMAND>\n"; 
        continue; 
      }
      doArea(polys, arg);
    }
    else if (cmd == "MAX")
    {
      std::string arg;
      std::istringstream as(rest);
      if (!(as >> arg)) { 
        std::cout << "<INVALID COMMAND>\n"; 
        continue; 
      }
      doMax(polys, arg);
    }
    else if (cmd == "MIN")
    {
      std::string arg;
      std::istringstream as(rest);
      if (!(as >> arg)) { 
        std::cout << "<INVALID COMMAND>\n"; 
        continue; 
      }
      doMin(polys, arg);
    }
    else if (cmd == "COUNT")
    {
      std::string arg;
      std::istringstream as(rest);
      if (!(as >> arg)) { 
        std::cout << "<INVALID COMMAND>\n"; 
        continue; 
      }
      doCount(polys, arg);
    }
    else if (cmd == "LESSAREA")
    {
      doLessArea(polys, rest);
    }
    else if (cmd == "SAME")
    {
      doSame(polys, rest);
    }
    else
    {
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  return 0;
}