#include "DataStruct.h"

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

    for (size_t i = 0; i < p1.points.size(); ++i)
    {
        if (p1.points[i].x != p2.points[i].x || p1.points[i].y != p2.points[i].y)
        {
            return false;
        }
    }

    return true;
}

void PolygonManager::executeArea(const std::string& param) const
{
    if (param == "ODD")
    {
        double totalArea = 0.0;
        int count = 0;
        for (const auto& p : polygons_)
        {
            if (p.points.size() % 2 != 0)
            {
                totalArea += calculateArea(p);
                count++;
            }
        }
        if (count > 0)
        {
            std::cout.precision(1);
            std::cout << std::fixed << totalArea << '\n';
        }
        else
        {
            std::cout << "<INVALID COMMAND>\n";
        }
    }
    else if (param == "EVEN")
    {
        double totalArea = 0.0;
        int count = 0;
        for (const auto& p : polygons_)
        {
            if (p.points.size() % 2 == 0)
            {
                totalArea += calculateArea(p);
                count++;
            }
        }
        if (count > 0)
        {
            std::cout.precision(1);
            std::cout << std::fixed << totalArea << '\n';
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
            double totalArea = 0.0;
            for (const auto& p : polygons_)
            {
                totalArea += calculateArea(p);
            }
            std::cout.precision(1);
            std::cout << std::fixed << (totalArea / polygons_.size()) << '\n';
        }
    }
    else
    {
        try
        {
            int targetVertices = std::stoi(param);
            double totalArea = 0.0;
            int count = 0;
            for (const auto& p : polygons_)
            {
                if (static_cast<int>(p.points.size()) == targetVertices)
                {
                    totalArea += calculateArea(p);
                    count++;
                }
            }
            if (count > 0)
            {
                std::cout.precision(1);
                std::cout << std::fixed << totalArea << '\n';
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

void PolygonManager::executeLessArea() const
{
    int vertexCount = 0;
    std::cin >> vertexCount;

    Polygon polyParam;
    for (int i = 0; i < vertexCount; ++i)
    {
        std::string pStr;
        std::cin >> pStr;
        if (pStr.front() == '(' && pStr.back() == ')')
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
    int lessCount = 0;
    for (const auto& p : polygons_)
    {
        if (calculateArea(p) < targetArea)
        {
            lessCount++;
        }
    }
    std::cout << lessCount << '\n';
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
        if (pStr.front() == '(' && pStr.back() == ')')
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

    int currentSeq = 0;
    int maxSeq = 0;
    for (const auto& p : polygons_)
    {
        if (comparePolygons(p, polyParam))
        {
            currentSeq++;
            maxSeq = std::max(maxSeq, currentSeq);
        }
        else
        {
            currentSeq = 0;
        }
    }
    std::cout << maxSeq << '\n';
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
