#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <iomanip>
#include <cmath>

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
    return lhs.points == rhs.points;
}

std::istream& operator>>(std::istream& in, Point& point)
{
    char open = 0;
    char separator = 0;
    char close = 0;

    if (in >> open >> point.x >> separator >> point.y >> close)
    {
        if (open != '(' || separator != ';' || close != ')')
        {
            in.setstate(std::ios::failbit);
        }
    }

    return in;
}

std::istream& operator>>(std::istream& in, Polygon& polygon)
{
    int count = 0;

    if (!(in >> count) || count < 3)
    {
        in.setstate(std::ios::failbit);
        return in;
    }

    polygon.points.resize(count);

    for (Point& point : polygon.points)
    {
        if (!(in >> point))
        {
            return in;
        }
    }

    return in;
}

double getArea(const Polygon& polygon)
{
    long long sum = std::inner_product(
        polygon.points.begin(),
        polygon.points.end() - 1,
        polygon.points.begin() + 1,
        static_cast<long long>(polygon.points.back().x) *
            polygon.points.front().y -
        static_cast<long long>(polygon.points.back().y) *
            polygon.points.front().x,
        std::plus<long long>(),
        [](const Point& a, const Point& b)
        {
            return static_cast<long long>(a.x) * b.y -
                   static_cast<long long>(a.y) * b.x;
        });

    return std::abs(static_cast<double>(sum)) / 2.0;
}

bool isRightAngle(const Point& previous,
                  const Point& current,
                  const Point& next)
{
    long long x1 =
        static_cast<long long>(previous.x) - current.x;
    long long y1 =
        static_cast<long long>(previous.y) - current.y;

    long long x2 =
        static_cast<long long>(next.x) - current.x;
    long long y2 =
        static_cast<long long>(next.y) - current.y;

    return x1 * x2 + y1 * y2 == 0;
}

bool hasRightAngle(const Polygon& polygon)
{
    const std::size_t size = polygon.points.size();

    return std::any_of(
        polygon.points.begin(),
        polygon.points.end(),
        [&polygon, size](const Point& point)
        {
            auto current =
                std::find(polygon.points.begin(),
                          polygon.points.end(),
                          point);

            std::size_t index =
                std::distance(polygon.points.begin(), current);

            const Point& previous =
                polygon.points[(index + size - 1) % size];

            const Point& next =
                polygon.points[(index + 1) % size];

            return isRightAngle(previous, point, next);
        });
}

bool isPermutation(const Polygon& polygon,
                   const Polygon& target)
{
    if (polygon.points.size() != target.points.size())
    {
        return false;
    }

    std::vector<Point> first = polygon.points;
    std::vector<Point> second = target.points;

    auto comparePoints = [](const Point& lhs, const Point& rhs)
    {
        if (lhs.x != rhs.x)
        {
            return lhs.x < rhs.x;
        }

        return lhs.y < rhs.y;
    };

    std::sort(first.begin(), first.end(), comparePoints);
    std::sort(second.begin(), second.end(), comparePoints);

    return first == second;
}

void invalidCommand()
{
    std::cout << "<INVALID COMMAND>\n";
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Error: No file specified\n";
        return 1;
    }

    std::ifstream file(argv[1]);

    if (!file)
    {
        std::cerr << "Error: Cannot open file\n";
        return 1;
    }

    std::vector<Polygon> polygons;
    std::string line;

    while (std::getline(file, line))
    {
        std::istringstream input(line);
        Polygon polygon;

        if (input >> polygon)
        {
            std::string garbage;

            if (!(input >> garbage))
            {
                polygons.push_back(polygon);
            }
        }
    }

    std::cout << std::fixed << std::setprecision(1);

    while (std::getline(std::cin, line))
    {
        std::istringstream input(line);
        std::string command;

        if (!(input >> command))
        {
            continue;
        }

        if (command == "AREA")
        {
            std::string parameter;

            if (!(input >> parameter))
            {
                invalidCommand();
                continue;
            }

            if (parameter == "ODD")
            {
                double result = std::accumulate(
                    polygons.begin(),
                    polygons.end(),
                    0.0,
                    [](double sum, const Polygon& polygon)
                    {
                        if (polygon.points.size() % 2 != 0)
                        {
                            return sum + getArea(polygon);
                        }

                        return sum;
                    });

                std::cout << result << '\n';
            }
            else if (parameter == "EVEN")
            {
                double result = std::accumulate(
                    polygons.begin(),
                    polygons.end(),
                    0.0,
                    [](double sum, const Polygon& polygon)
                    {
                        if (polygon.points.size() % 2 == 0)
                        {
                            return sum + getArea(polygon);
                        }

                        return sum;
                    });

                std::cout << result << '\n';
            }
            else if (parameter == "MEAN")
            {
                if (polygons.empty())
                {
                    invalidCommand();
                    continue;
                }

                double result = std::accumulate(
                    polygons.begin(),
                    polygons.end(),
                    0.0,
                    [](double sum, const Polygon& polygon)
                    {
                        return sum + getArea(polygon);
                    });

                std::cout << result / polygons.size() << '\n';
            }
            else
            {
                try
                {
                    std::size_t vertexes = std::stoul(parameter);

                    double result = std::accumulate(
                        polygons.begin(),
                        polygons.end(),
                        0.0,
                        [vertexes](double sum, const Polygon& polygon)
                        {
                            if (polygon.points.size() == vertexes)
                            {
                                return sum + getArea(polygon);
                            }

                            return sum;
                        });

                    std::cout << result << '\n';
                }
                catch (...)
                {
                    invalidCommand();
                }
            }
        }
        else if (command == "MAX")
        {
            std::string parameter;

            if (!(input >> parameter) || polygons.empty())
            {
                invalidCommand();
                continue;
            }

            if (parameter == "AREA")
            {
                auto it = std::max_element(
                    polygons.begin(),
                    polygons.end(),
                    [](const Polygon& lhs, const Polygon& rhs)
                    {
                        return getArea(lhs) < getArea(rhs);
                    });

                std::cout << getArea(*it) << '\n';
            }
            else if (parameter == "VERTEXES")
            {
                auto it = std::max_element(
                    polygons.begin(),
                    polygons.end(),
                    [](const Polygon& lhs, const Polygon& rhs)
                    {
                        return lhs.points.size() < rhs.points.size();
                    });

                std::cout << it->points.size() << '\n';
            }
            else
            {
                invalidCommand();
            }
        }
        else if (command == "MIN")
        {
            std::string parameter;

            if (!(input >> parameter) || polygons.empty())
            {
                invalidCommand();
                continue;
            }

            if (parameter == "AREA")
            {
                auto it = std::min_element(
                    polygons.begin(),
                    polygons.end(),
                    [](const Polygon& lhs, const Polygon& rhs)
                    {
                        return getArea(lhs) < getArea(rhs);
                    });

                std::cout << getArea(*it) << '\n';
            }
            else if (parameter == "VERTEXES")
            {
                auto it = std::min_element(
                    polygons.begin(),
                    polygons.end(),
                    [](const Polygon& lhs, const Polygon& rhs)
                    {
                        return lhs.points.size() < rhs.points.size();
                    });

                std::cout << it->points.size() << '\n';
            }
            else
            {
                invalidCommand();
            }
        }
        else if (command == "COUNT")
        {
            std::string parameter;

            if (!(input >> parameter))
            {
                invalidCommand();
                continue;
            }

            if (parameter == "EVEN")
            {
                auto result = std::count_if(
                    polygons.begin(),
                    polygons.end(),
                    [](const Polygon& polygon)
                    {
                        return polygon.points.size() % 2 == 0;
                    });

                std::cout << result << '\n';
            }
            else if (parameter == "ODD")
            {
                auto result = std::count_if(
                    polygons.begin(),
                    polygons.end(),
                    [](const Polygon& polygon)
                    {
                        return polygon.points.size() % 2 != 0;
                    });

                std::cout << result << '\n';
            }
            else
            {
                try
                {
                    std::size_t vertexes = std::stoul(parameter);

                    auto result = std::count_if(
                        polygons.begin(),
                        polygons.end(),
                        [vertexes](const Polygon& polygon)
                        {
                            return polygon.points.size() == vertexes;
                        });

                    std::cout << result << '\n';
                }
                catch (...)
                {
                    invalidCommand();
                }
            }
        }
        else if (command == "PERMS")
        {
            Polygon target;

            if (!(input >> target))
            {
                invalidCommand();
                continue;
            }

            std::string garbage;

            if (input >> garbage)
            {
                invalidCommand();
                continue;
            }

            auto result = std::count_if(
                polygons.begin(),
                polygons.end(),
                [&target](const Polygon& polygon)
                {
                    return isPermutation(polygon, target);
                });

            std::cout << result << '\n';
        }
        else if (command == "RIGHTSHAPES")
        {
            std::string garbage;

            if (input >> garbage)
            {
                invalidCommand();
                continue;
            }

            auto result = std::count_if(
                polygons.begin(),
                polygons.end(),
                [](const Polygon& polygon)
                {
                    return hasRightAngle(polygon);
                });

            std::cout << result << '\n';
        }
        else
        {
            invalidCommand();
        }
    }

    return 0;
}
