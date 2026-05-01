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
#include <map>
#include <cctype>

struct Point
{
    int x;
    int y;
};

struct Polygon
{
    std::vector<Point> points;
};

bool operator==(const Point& lhs, const Point& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

bool operator==(const Polygon& lhs, const Polygon& rhs)
{
    if (lhs.points.size() != rhs.points.size())
    {
        return false;
    }
    return std::equal(lhs.points.cbegin(), lhs.points.cend(), rhs.points.cbegin());
}

std::istream& operator>>(std::istream& in, Point& point)
{
    char leftParen, semicolon, rightParen;
    if ((in >> leftParen) && leftParen == '(' &&
        (in >> point.x) &&
        (in >> semicolon) && semicolon == ';' &&
        (in >> point.y) &&
        (in >> rightParen) && rightParen == ')')
    {
        return in;
    }
    in.setstate(std::ios::failbit);
    return in;
}

std::istream& operator>>(std::istream& in, Polygon& polygon)
{
    polygon.points.clear();
    size_t count = 0;
    if (!(in >> count) || count < 3)
    {
        in.setstate(std::ios::failbit);
        return in;
    }
    for (size_t i = 0; i < count; ++i)
    {
        Point p;
        if (!(in >> p))
        {
            in.setstate(std::ios::failbit);
            polygon.points.clear();
            return in;
        }
        polygon.points.push_back(p);
    }
    return in;
}

void discardInvalidCommand(std::istream& in, std::ostream& out)
{
    out << "<INVALID COMMAND>\n";
    in.clear();
    std::string discard;
    std::getline(in, discard);
}

double computeArea(const Polygon& polygon)
{
    double area = 0.0;
    size_t n = polygon.points.size();
    for (size_t i = 0; i < n; ++i)
    {
        const Point& p1 = polygon.points[i];
        const Point& p2 = polygon.points[(i + 1) % n];
        area += (static_cast<double>(p1.x) * p2.y - static_cast<double>(p2.x) * p1.y);
    }
    return std::abs(area) / 2.0;
}

bool isEven(const Polygon& polygon) { return polygon.points.size() % 2 == 0; }
bool isOdd(const Polygon& polygon) { return polygon.points.size() % 2 != 0; }
bool hasNVertexes(const Polygon& polygon, size_t n) { return polygon.points.size() == n; }

long long getOrientation(const Point& p, const Point& q, const Point& r)
{
    long long val = static_cast<long long>(q.y - p.y) * (r.x - q.x) -
        static_cast<long long>(q.x - p.x) * (r.y - q.y);
    if (val == 0) return 0;
    return (val > 0) ? 1 : 2;
}

bool onSegment(const Point& p, const Point& q, const Point& r)
{
    return q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
        q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y);
}

bool doIntersect(const Point& p1, const Point& q1, const Point& p2, const Point& q2)
{
    long long o1 = getOrientation(p1, q1, p2);
    long long o2 = getOrientation(p1, q1, q2);
    long long o3 = getOrientation(p2, q2, p1);
    long long o4 = getOrientation(p2, q2, q1);

    if (o1 != o2 && o3 != o4) return true;
    if (o1 == 0 && onSegment(p1, p2, q1)) return true;
    if (o2 == 0 && onSegment(p1, q2, q1)) return true;
    if (o3 == 0 && onSegment(p2, p1, q2)) return true;
    if (o4 == 0 && onSegment(p2, q1, q2)) return true;
    return false;
}

bool isPointInside(const Polygon& poly, const Point& pt)
{
    size_t crossings = 0;
    size_t n = poly.points.size();
    for (size_t i = 0; i < n; ++i)
    {
        const Point& p1 = poly.points[i];
        const Point& p2 = poly.points[(i + 1) % n];

        bool cond1 = (p1.y > pt.y) != (p2.y > pt.y);
        if (!cond1) continue;

        double intersectX = static_cast<double>(p2.x - p1.x) * (pt.y - p1.y) / (p2.y - p1.y) + p1.x;
        if (static_cast<double>(pt.x) < intersectX)
        {
            ++crossings;
        }
    }
    return crossings % 2 != 0;
}

bool polygonsIntersectEdges(const Polygon& poly1, const Polygon& poly2)
{
    size_t n1 = poly1.points.size();
    size_t n2 = poly2.points.size();

    for (size_t i = 0; i < n1; ++i)
    {
        Point p1 = poly1.points[i];
        Point q1 = poly1.points[(i + 1) % n1];
        for (size_t j = 0; j < n2; ++j)
        {
            Point p2 = poly2.points[j];
            Point q2 = poly2.points[(j + 1) % n2];
            if (doIntersect(p1, q1, p2, q2)) return true;
        }
    }
    return false;
}

bool isPolygonInside(const Polygon& poly1, const Polygon& poly2)
{
    return std::all_of(poly1.points.cbegin(), poly1.points.cend(), [&](const Point& pt) {
        return isPointInside(poly2, pt);
        });
}

bool polygonsIntersect(const Polygon& p1, const Polygon& p2)
{
    if (polygonsIntersectEdges(p1, p2)) return true;
    if (isPolygonInside(p1, p2)) return true;
    if (isPolygonInside(p2, p1)) return true;
    return false;
}

void cmdArea(std::vector<Polygon>& polygons, std::istream& in, std::ostream& out)
{
    std::string param;
    if (!(in >> param)) { discardInvalidCommand(in, out); return; }
    out << std::fixed << std::setprecision(1);

    if (param == "EVEN") {
        out << std::accumulate(polygons.cbegin(), polygons.cend(), 0.0, [](double sum, const Polygon& p) {
            return isEven(p) ? sum + computeArea(p) : sum;
            }) << "\n";
    }
    else if (param == "ODD") {
        out << std::accumulate(polygons.cbegin(), polygons.cend(), 0.0, [](double sum, const Polygon& p) {
            return isOdd(p) ? sum + computeArea(p) : sum;
            }) << "\n";
    }
    else if (param == "MEAN") {
        if (polygons.empty()) { discardInvalidCommand(in, out); return; }
        out << std::accumulate(polygons.cbegin(), polygons.cend(), 0.0, [](double sum, const Polygon& p) {
            return sum + computeArea(p);
            }) / polygons.size() << "\n";
    }
    else {
        if (param.empty() || !std::all_of(param.cbegin(), param.cend(), [](unsigned char c) { return std::isdigit(c); })) {
            discardInvalidCommand(in, out); return;
        }
        size_t vertexCount = std::stoull(param);
        if (vertexCount < 3) { discardInvalidCommand(in, out); return; }
        out << std::accumulate(polygons.cbegin(), polygons.cend(), 0.0, [vertexCount](double sum, const Polygon& p) {
            return hasNVertexes(p, vertexCount) ? sum + computeArea(p) : sum;
            }) << "\n";
    }
}

void cmdMax(std::vector<Polygon>& polygons, std::istream& in, std::ostream& out)
{
    std::string param;
    if (!(in >> param) || polygons.empty()) { discardInvalidCommand(in, out); return; }

    if (param == "AREA") {
        auto it = std::max_element(polygons.cbegin(), polygons.cend(), [](const Polygon& a, const Polygon& b) {
            return computeArea(a) < computeArea(b);
            });
        out << std::fixed << std::setprecision(1) << computeArea(*it) << "\n";
    }
    else if (param == "VERTEXES") {
        auto it = std::max_element(polygons.cbegin(), polygons.cend(), [](const Polygon& a, const Polygon& b) {
            return a.points.size() < b.points.size();
            });
        out << it->points.size() << "\n";
    }
    else {
        discardInvalidCommand(in, out);
    }
}

void cmdMin(std::vector<Polygon>& polygons, std::istream& in, std::ostream& out)
{
    std::string param;
    if (!(in >> param) || polygons.empty()) { discardInvalidCommand(in, out); return; }

    if (param == "AREA") {
        auto it = std::min_element(polygons.cbegin(), polygons.cend(), [](const Polygon& a, const Polygon& b) {
            return computeArea(a) < computeArea(b);
            });
        out << std::fixed << std::setprecision(1) << computeArea(*it) << "\n";
    }
    else if (param == "VERTEXES") {
        auto it = std::min_element(polygons.cbegin(), polygons.cend(), [](const Polygon& a, const Polygon& b) {
            return a.points.size() < b.points.size();
            });
        out << it->points.size() << "\n";
    }
    else {
        discardInvalidCommand(in, out);
    }
}

void cmdCount(std::vector<Polygon>& polygons, std::istream& in, std::ostream& out)
{
    std::string param;
    if (!(in >> param)) { discardInvalidCommand(in, out); return; }

    if (param == "EVEN") {
        out << std::count_if(polygons.cbegin(), polygons.cend(), isEven) << "\n";
    }
    else if (param == "ODD") {
        out << std::count_if(polygons.cbegin(), polygons.cend(), isOdd) << "\n";
    }
    else {
        if (param.empty() || !std::all_of(param.cbegin(), param.cend(), [](unsigned char c) { return std::isdigit(c); })) {
            discardInvalidCommand(in, out); return;
        }
        size_t vertexCount = std::stoull(param);
        if (vertexCount < 3) { discardInvalidCommand(in, out); return; }
        out << std::count_if(polygons.cbegin(), polygons.cend(), [vertexCount](const Polygon& p) {
            return hasNVertexes(p, vertexCount);
            }) << "\n";
    }
}

void cmdRmEcho(std::vector<Polygon>& polygons, std::istream& in, std::ostream& out)
{
    Polygon target;
    if (!(in >> target)) {
        discardInvalidCommand(in, out);
        return;
    }
    auto newEnd = std::remove_if(polygons.begin(), polygons.end(), [&](const Polygon& p) {
        return p == target;
        });
    out << std::distance(newEnd, polygons.end()) << "\n";
    polygons.erase(newEnd, polygons.end());
}

void cmdIntersections(std::vector<Polygon>& polygons, std::istream& in, std::ostream& out)
{
    Polygon target;
    if (!(in >> target)) {
        discardInvalidCommand(in, out);
        return;
    }
    out << std::count_if(polygons.cbegin(), polygons.cend(), [&](const Polygon& p) {
        return polygonsIntersect(p, target);
        }) << "\n";
}

int main(int argc, char* argv[])
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

    std::vector<Polygon> polygons;
    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream ss(line);
        Polygon polygon;
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

    using CommandFunc = std::function<void(std::vector<Polygon>&, std::istream&, std::ostream&)>;
    std::map<std::string, CommandFunc> commands = {
        {"AREA", cmdArea},
        {"MAX", cmdMax},
        {"MIN", cmdMin},
        {"COUNT", cmdCount},
        {"RMECHO", cmdRmEcho},
        {"INTERSECTIONS", cmdIntersections}
    };

    std::string commandName;
    while (std::cin >> commandName)
    {
        auto it = commands.find(commandName);
        if (it != commands.end())
        {
            it->second(polygons, std::cin, std::cout);
        }
        else
        {
            discardInvalidCommand(std::cin, std::cout);
        }
    }
    return 0;
}
