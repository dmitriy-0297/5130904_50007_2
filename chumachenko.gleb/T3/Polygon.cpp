#include "Polygon.h"
#include <numeric>
#include <algorithm>
#include <cmath>

iofmtguard::iofmtguard(std::basic_ios< char >& s) :
    s_(s),
    width_(s.width()),
    fill_(s.fill()),
    precision_(s.precision()),
    fmt_(s.flags())
{
}

iofmtguard::~iofmtguard()
{
    s_.width(width_);
    s_.fill(fill_);
    s_.precision(precision_);
    s_.flags(fmt_);
}

bool operator==(const Point& left, const Point& right)
{
    return (left.x == right.x) && (left.y == right.y);
}

bool operator==(const Polygon& left, const Polygon& right)
{
    if (left.points.size() != right.points.size())
    {
        return false;
    }
    return std::equal(left.points.begin(), left.points.end(), right.points.begin());
}

std::istream& operator>>(std::istream& in, Point& point)
{
    char openParen = '\0';
    char semicolon = '\0';
    char closeParen = '\0';
    int x = 0;
    int y = 0;
    if (in >> openParen >> x >> semicolon >> y >> closeParen)
    {
        if (openParen == '(' && semicolon == ';' && closeParen == ')')
        {
            point.x = x;
            point.y = y;
        }
        else
        {
            in.setstate(std::ios::failbit);
        }
    }
    return in;
}

std::istream& operator>>(std::istream& in, Polygon& polygon)
{
    int count = 0;
    if (in >> count)
    {
        if (count < 3)
        {
            in.setstate(std::ios::failbit);
            return in;
        }
        std::vector< Point > tempPoints;
        auto gen = [&in]()
        {
            Point p = {0, 0};
            if (in)
            {
                in >> p;
            }
            return p;
        };
        std::generate_n(std::back_inserter(tempPoints), count, gen);
        if (in)
        {
            polygon.points = tempPoints;
        }
    }
    return in;
}

double calculatePolygonArea(const Polygon& polygon)
{
    if (polygon.points.size() < 3)
    {
        return 0.0;
    }
    std::vector< double > crossProducts;
    std::vector< Point > shifted = polygon.points;
    std::rotate(shifted.begin(), shifted.begin() + 1, shifted.end());
    auto op = [](const Point& p1, const Point& p2)
    {
        double first = static_cast< double >(p1.x * p2.y);
        double second = static_cast< double >(p2.x * p1.y);
        return first - second;
    };
    std::transform(polygon.points.begin(), polygon.points.end(),
        shifted.begin(), std::back_inserter(crossProducts), op);
    double areaSum = std::accumulate(crossProducts.begin(), crossProducts.end(), 0.0);
    return std::abs(areaSum) / 2.0;
}

bool isRectangle(const Polygon& polygon)
{
    if (polygon.points.size() != 4)
    {
        return false;
    }
    auto checkRightAngle = [](const Point& p1, const Point& p2, const Point& p3)
    {
        int v1x = p1.x - p2.x;
        int v1y = p1.y - p2.y;
        int v2x = p3.x - p2.x;
        int v2y = p3.y - p2.y;
        return (v1x * v2x + v1y * v2y) == 0;
    };
    bool r1 = checkRightAngle(polygon.points[0], polygon.points[1], polygon.points[2]);
    bool r2 = checkRightAngle(polygon.points[1], polygon.points[2], polygon.points[3]);
    bool r3 = checkRightAngle(polygon.points[2], polygon.points[3], polygon.points[0]);
    bool r4 = checkRightAngle(polygon.points[3], polygon.points[0], polygon.points[1]);
    return r1 && r2 && r3 && r4;
}

void printInvalidCommand()
{
    std::cout << "<INVALID COMMAND>\n";
}

void processArea(const std::vector< Polygon >& polygons, std::istream& in)
{
    std::string arg;
    in >> arg;
    if (arg == "EVEN")
    {
        auto op = [](double acc, const Polygon& p)
        {
            if (p.points.size() % 2 == 0)
            {
                return acc + calculatePolygonArea(p);
            }
            return acc;
        };
        double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0, op);
        iofmtguard guard(std::cout);
        std::cout << std::fixed << std::setprecision(1) << sum << '\n';
    }
    else if (arg == "ODD")
    {
        auto op = [](double acc, const Polygon& p)
        {
            if (p.points.size() % 2 != 0)
            {
                return acc + calculatePolygonArea(p);
            }
            return acc;
        };
        double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0, op);
        iofmtguard guard(std::cout);
        std::cout << std::fixed << std::setprecision(1) << sum << '\n';
    }
    else if (arg == "MEAN")
    {
        if (polygons.empty())
        {
            printInvalidCommand();
            return;
        }
        auto op = [](double acc, const Polygon& p)
        {
            return acc + calculatePolygonArea(p);
        };
        double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0, op);
        iofmtguard guard(std::cout);
        std::cout << std::fixed << std::setprecision(1) << sum / static_cast< double >(polygons.size()) << '\n';
    }
    else
    {
        try
        {
            size_t sz = std::stoull(arg);
            if (sz < 3)
            {
                printInvalidCommand();
                return;
            }
            auto op = [sz](double acc, const Polygon& p)
            {
                if (p.points.size() == sz)
                {
                    return acc + calculatePolygonArea(p);
                }
                return acc;
            };
            double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0, op);
            iofmtguard guard(std::cout);
            std::cout << std::fixed << std::setprecision(1) << sum << '\n';
        }
        catch (...)
        {
            printInvalidCommand();
        }
    }
}

void processMax(const std::vector< Polygon >& polygons, std::istream& in)
{
    std::string arg;
    in >> arg;
    if (polygons.empty())
    {
        printInvalidCommand();
        return;
    }
    if (arg == "AREA")
    {
        auto cmp = [](const Polygon& p1, const Polygon& p2)
        {
            return calculatePolygonArea(p1) < calculatePolygonArea(p2);
        };
        auto it = std::max_element(polygons.begin(), polygons.end(), cmp);
        iofmtguard guard(std::cout);
        std::cout << std::fixed << std::setprecision(1) << calculatePolygonArea(*it) << '\n';
    }
    else if (arg == "VERTEXES")
    {
        auto cmp = [](const Polygon& p1, const Polygon& p2)
        {
            return p1.points.size() < p2.points.size();
        };
        auto it = std::max_element(polygons.begin(), polygons.end(), cmp);
        std::cout << it->points.size() << '\n';
    }
    else
    {
        printInvalidCommand();
    }
}

void processMin(const std::vector< Polygon >& polygons, std::istream& in)
{
    std::string arg;
    in >> arg;
    if (polygons.empty())
    {
        printInvalidCommand();
        return;
    }
    if (arg == "AREA")
    {
        auto cmp = [](const Polygon& p1, const Polygon& p2)
        {
            return calculatePolygonArea(p1) < calculatePolygonArea(p2);
        };
        auto it = std::min_element(polygons.begin(), polygons.end(), cmp);
        iofmtguard guard(std::cout);
        std::cout << std::fixed << std::setprecision(1) << calculatePolygonArea(*it) << '\n';
    }
    else if (arg == "VERTEXES")
    {
        auto cmp = [](const Polygon& p1, const Polygon& p2)
        {
            return p1.points.size() < p2.points.size();
        };
        auto it = std::min_element(polygons.begin(), polygons.end(), cmp);
        std::cout << it->points.size() << '\n';
    }
    else
    {
        printInvalidCommand();
    }
}

void processCount(const std::vector< Polygon >& polygons, std::istream& in)
{
    std::string arg;
    in >> arg;
    if (arg == "EVEN")
    {
        auto op = [](const Polygon& p)
        {
            return p.points.size() % 2 == 0;
        };
        std::cout << std::count_if(polygons.begin(), polygons.end(), op) << '\n';
    }
    else if (arg == "ODD")
    {
        auto op = [](const Polygon& p)
        {
            return p.points.size() % 2 != 0;
        };
        std::cout << std::count_if(polygons.begin(), polygons.end(), op) << '\n';
    }
    else
    {
        try
        {
            size_t sz = std::stoull(arg);
            if (sz < 3)
            {
                printInvalidCommand();
                return;
            }
            auto op = [sz](const Polygon& p)
            {
                return p.points.size() == sz;
            };
            std::cout << std::count_if(polygons.begin(), polygons.end(), op) << '\n';
        }
        catch (...)
        {
            printInvalidCommand();
        }
    }
}

void processRects(const std::vector< Polygon >& polygons)
{
    std::cout << std::count_if(polygons.begin(), polygons.end(), isRectangle) << '\n';
}

struct MaxSeqState
{
    int maxCount_;
    int currentCount_;
    Polygon target_;
};

void processMaxSeq(const std::vector< Polygon >& polygons, std::istream& in)
{
    Polygon target;
    if (in >> target)
    {
        std::string garbage;
        if (in >> garbage)
        {
            printInvalidCommand();
            return;
        }
        MaxSeqState init = {0, 0, target};
        auto op = [](MaxSeqState s, const Polygon& p)
        {
            if (p == s.target_)
            {
                s.currentCount_++;
                s.maxCount_ = std::max(s.maxCount_, s.currentCount_);
            }
            else
            {
                s.currentCount_ = 0;
            }
            return s;
        };
        MaxSeqState res = std::accumulate(polygons.begin(), polygons.end(), init, op);
        std::cout << res.maxCount_ << '\n';
    }
    else
    {
        printInvalidCommand();
    }
}

