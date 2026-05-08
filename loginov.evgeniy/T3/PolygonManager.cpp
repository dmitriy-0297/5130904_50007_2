#include "PolygonManager.h"

PolygonManager::PolygonManager(const std::vector<Polygon>& polygons) :
    polygons_(polygons)
{
}

double PolygonManager::calculateArea(const Polygon& polygon) const
{
    double areaValue = 0.0;
    size_t n = polygon.points.size();
    if (n < 3)
    {
        return 0.0;
    }
    for (size_t i = 0; i < n; ++i)
    {
        size_t next = (i + 1) % n;
        areaValue += static_cast<double>(polygon.points[i].x * polygon.points[next].y);
        areaValue -= static_cast<double>(polygon.points[next].x * polygon.points[i].y);
    }
    return std::abs(areaValue) * 0.5;
}

bool PolygonManager::comparePolygons(const Polygon& p1, const Polygon& p2) const
{
    if (p1.points.size() != p2.points.size())
    {
        return false;
    }
    return std::equal(p1.points.begin(), p1.points.end(), p2.points.begin(),
        [](const Point& a, const Point& b)
        {
            return a.x == b.x && a.y == b.y;
        });
}

void PolygonManager::executeArea(const std::string& param) const
{
    auto areaCalc = std::bind(&PolygonManager::calculateArea, this, std::placeholders::_1);

    if (param == "ODD")
    {
        auto predicate = [](const Polygon& p) { return p.points.size() % 2 != 0; };
        double totalArea = std::accumulate(polygons_.begin(), polygons_.end(), 0.0,
            [&](double acc, const Polygon& p)
            {
                return predicate(p) ? acc + areaCalc(p) : acc;
            });
        int count = std::count_if(polygons_.begin(), polygons_.end(), predicate);
        if (count > 0)
        {
            std::cout << std::fixed << std::setprecision(1) << totalArea << '\n';
        }
        else
        {
            std::cout << "<INVALID COMMAND>\n";
        }
    }
    else if (param == "EVEN")
    {
        auto predicate = [](const Polygon& p) { return p.points.size() % 2 == 0; };
        double totalArea = std::accumulate(polygons_.begin(), polygons_.end(), 0.0,
            [&](double acc, const Polygon& p)
            {
                return predicate(p) ? acc + areaCalc(p) : acc;
            });
        int count = std::count_if(polygons_.begin(), polygons_.end(), predicate);
        if (count > 0)
        {
            std::cout << std::fixed << std::setprecision(1) << totalArea << '\n';
        }
        else
        {
            std::cout << "<INVALID COMMAND>\n";
        }
    }
    else if (param == "MEAN")
    {
        if (polygons_.empty())
        {
            std::cout << "<INVALID COMMAND>\n";
        }
        else
        {
            double totalArea = std::accumulate(polygons_.begin(), polygons_.end(), 0.0,
                [&](double acc, const Polygon& p) { return acc + areaCalc(p); });
            std::cout << std::fixed << std::setprecision(1) << (totalArea / polygons_.size()) << '\n';
        }
    }
    else
    {
        try
        {
            int targetVertices = std::stoi(param);
            auto predicate = [targetVertices](const Polygon& p)
            {
                return static_cast<int>(p.points.size()) == targetVertices;
            };
            double totalArea = std::accumulate(polygons_.begin(), polygons_.end(), 0.0,
                [&](double acc, const Polygon& p)
                {
                    return predicate(p) ? acc + areaCalc(p) : acc;
                });
            int count = std::count_if(polygons_.begin(), polygons_.end(), predicate);
            if (count > 0)
            {
                std::cout << std::fixed << std::setprecision(1) << totalArea << '\n';
            }
            else
            {
                std::cout << "<INVALID COMMAND>\n";
            }
        }
        catch (...)
        {
            std::cout << "<INVALID COMMAND>\n";
        }
    }
}

void PolygonManager::executeMax(const std::string& param) const
{
    if (polygons_.empty())
    {
        std::cout << "<INVALID COMMAND>\n";
        return;
    }

    if (param == "AREA")
    {
        auto areaCalc = std::bind(&PolygonManager::calculateArea, this, std::placeholders::_1);
        auto it = std::max_element(polygons_.begin(), polygons_.end(),
            [&](const Polygon& a, const Polygon& b)
            {
                return areaCalc(a) < areaCalc(b);
            });
        std::cout << std::fixed << std::setprecision(1) << calculateArea(*it) << '\n';
    }
    else if (param == "VERTEXES")
    {
        auto it = std::max_element(polygons_.begin(), polygons_.end(),
            [](const Polygon& a, const Polygon& b)
            {
                return a.points.size() < b.points.size();
            });
        std::cout << it->points.size() << '\n';
    }
    else
    {
        std::cout << "<INVALID COMMAND>\n";
    }
}

void PolygonManager::executeMin(const std::string& param) const
{
    if (polygons_.empty())
    {
        std::cout << "<INVALID COMMAND>\n";
        return;
    }

    if (param == "AREA")
    {
        auto areaCalc = std::bind(&PolygonManager::calculateArea, this, std::placeholders::_1);
        auto it = std::min_element(polygons_.begin(), polygons_.end(),
            [&](const Polygon& a, const Polygon& b)
            {
                return areaCalc(a) < areaCalc(b);
            });
        std::cout << std::fixed << std::setprecision(1) << calculateArea(*it) << '\n';
    }
    else if (param == "VERTEXES")
    {
        auto it = std::min_element(polygons_.begin(), polygons_.end(),
            [](const Polygon& a, const Polygon& b)
            {
                return a.points.size() < b.points.size();
            });
        std::cout << it->points.size() << '\n';
    }
    else
    {
        std::cout << "<INVALID COMMAND>\n";
    }
}

void PolygonManager::executeCount(const std::string& param) const
{
    try
    {
        std::function<bool(const Polygon&)> predicate;

        if (param == "EVEN")
        {
            predicate = [](const Polygon& p) { return p.points.size() % 2 == 0; };
        }
        else if (param == "ODD")
        {
            predicate = [](const Polygon& p) { return p.points.size() % 2 != 0; };
        }
        else
        {
            int target = std::stoi(param);
            predicate = [target](const Polygon& p)
            {
                return p.points.size() == static_cast<size_t>(target);
            };
        }

        std::cout << std::count_if(polygons_.begin(), polygons_.end(), predicate) << '\n';
    }
    catch (...)
    {
        std::cout << "<INVALID COMMAND>\n";
    }
}

void PolygonManager::executeLessArea() const
{
    int vertexCount = 0;
    std::cin >> vertexCount;
    Polygon polyParam;
    for (int i = 0; i < vertexCount; ++i)
    {
        std::string pStr;
        std::cin >> pStr;
        if (!pStr.empty() && pStr.front() == '(' && pStr.back() == ')')
        {
            pStr = pStr.substr(1, pStr.length() - 2);
        }
        size_t sep = pStr.find(';');
        if (sep != std::string::npos)
        {
            try
            {
                int xVal = std::stoi(pStr.substr(0, sep));
                int yVal = std::stoi(pStr.substr(sep + 1));
                polyParam.points.push_back({xVal, yVal});
            }
            catch (...)
            {
                std::cout << "<INVALID COMMAND>\n";
                return;
            }
        }
    }
    if (polyParam.points.size() != static_cast<size_t>(vertexCount))
    {
        std::cout << "<INVALID COMMAND>\n";
        return;
    }
    double targetArea = calculateArea(polyParam);
    auto areaCalc = std::bind(&PolygonManager::calculateArea, this, std::placeholders::_1);
    auto predicate = [targetArea, areaCalc](const Polygon& p)
    {
        return areaCalc(p) < targetArea;
    };
    std::cout << std::count_if(polygons_.begin(), polygons_.end(), predicate) << '\n';
}

void PolygonManager::executeMaxSeq() const
{
    int vertexCount = 0;
    std::cin >> vertexCount;
    Polygon polyParam;
    for (int i = 0; i < vertexCount; ++i)
    {
        std::string pStr;
        std::cin >> pStr;
        if (!pStr.empty() && pStr.front() == '(' && pStr.back() == ')')
        {
            pStr = pStr.substr(1, pStr.length() - 2);
        }
        size_t sep = pStr.find(';');
        if (sep != std::string::npos)
        {
            try
            {
                int xVal = std::stoi(pStr.substr(0, sep));
                int yVal = std::stoi(pStr.substr(sep + 1));
                polyParam.points.push_back({xVal, yVal});
            }
            catch (...)
            {
                std::cout << "<INVALID COMMAND>\n";
                return;
            }
        }
    }
    if (polyParam.points.size() != static_cast<size_t>(vertexCount))
    {
        std::cout << "<INVALID COMMAND>\n";
        return;
    }
    auto comparator = std::bind(&PolygonManager::comparePolygons, this, std::placeholders::_1, polyParam);
    auto result = std::accumulate(polygons_.begin(), polygons_.end(), std::make_pair(0, 0),
        [&](std::pair<int, int> state, const Polygon& p)
        {
            if (comparator(p))
            {
                state.first++;
                state.second = std::max(state.second, state.first);
            }
            else
            {
                state.first = 0;
            }
            return state;
        });
    std::cout << result.second << '\n';
}

void PolygonManager::processCommands() const
{
    std::string command;
    while (std::cin >> command)
    {
        if (command == "AREA")
        {
            std::string param;
            std::cin >> param;
            executeArea(param);
        }
        else if (command == "MAX")
        {
            std::string param;
            std::cin >> param;
            executeMax(param);
        }
        else if (command == "MIN")
        {
            std::string param;
            std::cin >> param;
            executeMin(param);
        }
        else if (command == "COUNT")
        {
            std::string param;
            std::cin >> param;
            executeCount(param);
        }
        else if (command == "LESSAREA")
        {
            executeLessArea();
        }
        else if (command == "MAXSEQ")
        {
            executeMaxSeq();
        }
        else
        {
            std::cout << "<INVALID COMMAND>\n";
        }
    }
}