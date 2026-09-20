#include "Polygon.h"

#include <cctype>
#include <iterator>

bool operator==(const Point& lhs, const Point& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

bool operator==(const Polygon& lhs, const Polygon& rhs)
{
    return lhs.points == rhs.points;
}

double getArea(const Polygon& polygon)
{
    const std::size_t size = polygon.points.size();

    if (size < 3)
        return 0.0;

    std::vector<std::size_t> indexes(size);
    std::iota(indexes.begin(), indexes.end(), 0);

    const long long sum = std::accumulate(
        indexes.begin(),
        indexes.end(),
        0LL,
        [&polygon](long long current, std::size_t index)
        {
            const std::size_t next =
                (index + 1) % polygon.points.size();

            return current
                + static_cast<long long>(polygon.points[index].x)
                * polygon.points[next].y
                - static_cast<long long>(polygon.points[next].x)
                * polygon.points[index].y;
        });

    return std::abs(static_cast<double>(sum)) / 2.0;
}

static bool isNumber(const std::string& str)
{
    if (str.empty())
        return false;

    return std::all_of(
        str.begin(),
        str.end(),
        [](char ch)
        {
            return std::isdigit(static_cast<unsigned char>(ch));
        });
}

static bool parsePoint(const std::string& token, Point& point)
{
    if (token.size() < 5)
        return false;

    if (token.front() != '(' || token.back() != ')')
        return false;

    const std::string content =
        token.substr(1, token.size() - 2);

    const std::size_t separator = content.find(';');

    if (separator == std::string::npos)
        return false;

    if (content.find(';', separator + 1) != std::string::npos)
        return false;

    const std::string xStr = content.substr(0, separator);
    const std::string yStr = content.substr(separator + 1);

    if (!isNumber(xStr) && !(xStr.size() > 1 && xStr[0] == '-'
        && isNumber(xStr.substr(1))))
        return false;

    if (!isNumber(yStr) && !(yStr.size() > 1 && yStr[0] == '-'
        && isNumber(yStr.substr(1))))
        return false;

    try
    {
        point.x = std::stoi(xStr);
        point.y = std::stoi(yStr);
    }
    catch (const std::exception&)
    {
        return false;
    }

    return true;
}

static std::vector<std::string> splitBySingleSpaces(
    const std::string& str)
{
    if (str.empty())
        return {};

    if (str.front() == ' ' || str.back() == ' ')
        return { "" };

    const auto invalidWhitespace = std::find_if(
        str.begin(),
        str.end(),
        [](char ch)
        {
            return std::isspace(static_cast<unsigned char>(ch))
                && ch != ' ';
        });

    if (invalidWhitespace != str.end())
        return { "" };

    const auto doubleSpace = std::adjacent_find(
        str.begin(),
        str.end(),
        [](char lhs, char rhs)
        {
            return lhs == ' ' && rhs == ' ';
        });

    if (doubleSpace != str.end())
        return { "" };

    std::istringstream input(str);

    return std::vector<std::string>(
        std::istream_iterator<std::string>(input),
        std::istream_iterator<std::string>());
}

bool parsePolygon(const std::string& line, Polygon& polygon)
{
    const std::vector<std::string> tokens =
        splitBySingleSpaces(line);

    if (tokens.empty() || tokens.size() < 4)
        return false;

    if (!isNumber(tokens[0]))
        return false;

    std::size_t vertexCount = 0;

    try
    {
        vertexCount = std::stoull(tokens[0]);
    }
    catch (const std::exception&)
    {
        return false;
    }

    if (vertexCount < 3)
        return false;

    if (tokens.size() != vertexCount + 1)
        return false;

    std::vector<Point> points(vertexCount);

    std::vector<std::size_t> indexes(vertexCount);
    std::iota(indexes.begin(), indexes.end(), 0);

    const bool valid = std::all_of(
        indexes.begin(),
        indexes.end(),
        [&tokens, &points](std::size_t index)
        {
            return parsePoint(tokens[index + 1], points[index]);
        });

    if (!valid)
        return false;

    polygon.points = std::move(points);
    return true;
}

bool samePolygon(const Polygon& lhs, const Polygon& rhs)
{
    if (lhs.points.size() != rhs.points.size())
        return false;

    if (lhs.points.empty())
        return true;

    const std::size_t size = lhs.points.size();

    std::vector<std::size_t> shifts(size);
    std::iota(shifts.begin(), shifts.end(), 0);

    return std::any_of(
        shifts.begin(),
        shifts.end(),
        [&lhs, &rhs, size](std::size_t shift)
        {
            const int dx =
                rhs.points[shift].x - lhs.points[0].x;

            const int dy =
                rhs.points[shift].y - lhs.points[0].y;

            std::vector<std::size_t> indexes(size);
            std::iota(indexes.begin(), indexes.end(), 0);

            return std::all_of(
                indexes.begin(),
                indexes.end(),
                [&lhs, &rhs, shift, dx, dy, size](std::size_t index)
                {
                    const std::size_t rhsIndex =
                        (shift + index) % size;

                    return lhs.points[index].x + dx
                        == rhs.points[rhsIndex].x
                        && lhs.points[index].y + dy
                        == rhs.points[rhsIndex].y;
                });
        });
}

static void printArea(double area)
{
    std::cout << std::fixed
        << std::setprecision(1)
        << area
        << '\n';
}

void processCommand(const std::string& command,
    std::vector<Polygon>& polygons)
{
    const std::vector<std::string> tokens =
        splitBySingleSpaces(command);

    if (tokens.empty())
    {
        std::cout << "<INVALID COMMAND>\n";
        return;
    }

    if (tokens[0] == "AREA")
    {
        if (tokens.size() != 2)
        {
            std::cout << "<INVALID COMMAND>\n";
            return;
        }

        if (tokens[1] == "EVEN" || tokens[1] == "ODD")
        {
            const bool even = tokens[1] == "EVEN";

            const double result = std::accumulate(
                polygons.begin(),
                polygons.end(),
                0.0,
                [even](double sum, const Polygon& polygon)
                {
                    const bool polygonEven =
                        polygon.points.size() % 2 == 0;

                    if (polygonEven == even)
                        return sum + getArea(polygon);

                    return sum;
                });

            printArea(result);
            return;
        }

        if (tokens[1] == "MEAN")
        {
            if (polygons.empty())
            {
                std::cout << "<INVALID COMMAND>\n";
                return;
            }

            const double total = std::accumulate(
                polygons.begin(),
                polygons.end(),
                0.0,
                [](double sum, const Polygon& polygon)
                {
                    return sum + getArea(polygon);
                });

            printArea(total / polygons.size());
            return;
        }

        if (isNumber(tokens[1]))
        {
            std::size_t vertexCount = 0;

            try
            {
                vertexCount = std::stoull(tokens[1]);
            }
            catch (const std::exception&)
            {
                std::cout << "<INVALID COMMAND>\n";
                return;
            }

            const double result = std::accumulate(
                polygons.begin(),
                polygons.end(),
                0.0,
                [vertexCount](double sum,
                    const Polygon& polygon)
                {
                    if (polygon.points.size() == vertexCount)
                        return sum + getArea(polygon);

                    return sum;
                });

            printArea(result);
            return;
        }

        std::cout << "<INVALID COMMAND>\n";
        return;
    }

    if (tokens[0] == "MAX" || tokens[0] == "MIN")
    {
        if (tokens.size() != 2 || polygons.empty())
        {
            std::cout << "<INVALID COMMAND>\n";
            return;
        }

        if (tokens[1] == "AREA")
        {
            const auto comparator =
                [](const Polygon& lhs, const Polygon& rhs)
                {
                    return getArea(lhs) < getArea(rhs);
                };

            const auto iterator =
                tokens[0] == "MAX"
                ? std::max_element(
                    polygons.begin(),
                    polygons.end(),
                    comparator)
                : std::min_element(
                    polygons.begin(),
                    polygons.end(),
                    comparator);

            printArea(getArea(*iterator));
            return;
        }

        if (tokens[1] == "VERTEXES")
        {
            const auto comparator =
                [](const Polygon& lhs, const Polygon& rhs)
                {
                    return lhs.points.size()
                        < rhs.points.size();
                };

            const auto iterator =
                tokens[0] == "MAX"
                ? std::max_element(
                    polygons.begin(),
                    polygons.end(),
                    comparator)
                : std::min_element(
                    polygons.begin(),
                    polygons.end(),
                    comparator);

            std::cout << iterator->points.size() << '\n';
            return;
        }

        std::cout << "<INVALID COMMAND>\n";
        return;
    }

    if (tokens[0] == "COUNT")
    {
        if (tokens.size() != 2)
        {
            std::cout << "<INVALID COMMAND>\n";
            return;
        }

        if (tokens[1] == "EVEN" || tokens[1] == "ODD")
        {
            const bool even = tokens[1] == "EVEN";

            const auto count = std::count_if(
                polygons.begin(),
                polygons.end(),
                [even](const Polygon& polygon)
                {
                    return (polygon.points.size() % 2 == 0)
                        == even;
                });

            std::cout << count << '\n';
            return;
        }

        if (isNumber(tokens[1]))
        {
            std::size_t vertexCount = 0;

            try
            {
                vertexCount = std::stoull(tokens[1]);
            }
            catch (const std::exception&)
            {
                std::cout << "<INVALID COMMAND>\n";
                return;
            }

            const auto count = std::count_if(
                polygons.begin(),
                polygons.end(),
                VertexCount{ vertexCount });

            std::cout << count << '\n';
            return;
        }

        std::cout << "<INVALID COMMAND>\n";
        return;
    }

    if (tokens[0] == "RMECHO")
    {
        if (tokens.size() != 2)
        {
            std::cout << "<INVALID COMMAND>\n";
            return;
        }

        Polygon target;

        if (!parsePolygon(tokens[1], target))
        {
            std::cout << "<INVALID COMMAND>\n";
            return;
        }

        bool previousWasTarget = false;

        const auto newEnd = std::remove_if(
            polygons.begin(),
            polygons.end(),
            [&target, &previousWasTarget](const Polygon& polygon)
            {
                if (polygon == target)
                {
                    const bool remove = previousWasTarget;
                    previousWasTarget = true;
                    return remove;
                }

                previousWasTarget = false;
                return false;
            });

        const std::size_t removed =
            static_cast<std::size_t>(
                std::distance(newEnd, polygons.end()));

        polygons.erase(newEnd, polygons.end());

        std::cout << removed << '\n';
        return;
    }

    if (tokens[0] == "SAME")
    {
        if (tokens.size() != 2)
        {
            std::cout << "<INVALID COMMAND>\n";
            return;
        }

        Polygon target;

        if (!parsePolygon(tokens[1], target))
        {
            std::cout << "<INVALID COMMAND>\n";
            return;
        }

        const auto count = std::count_if(
            polygons.begin(),
            polygons.end(),
            Same{ target });

        std::cout << count << '\n';
        return;
    }

    std::cout << "<INVALID COMMAND>\n";
}