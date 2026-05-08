#include "PolygonManager.h"

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Error: No filename provided.\n";
        return 1;
    }

    std::ifstream inputFile(argv[1]);
    if (!inputFile.is_open())
    {
        std::cerr << "Error: Could not open file.\n";
        return 1;
    }

    std::vector<Polygon> polygons;
    std::string line;

    while (std::getline(inputFile, line))
    {
        if (line.empty())
        {
            continue;
        }

        // Find the first '(' to separate vertex count from points
        size_t firstParen = line.find('(');
        if (firstParen == std::string::npos)
        {
            continue;
        }

        // Extract vertex count
        int vertexCount = 0;
        try
        {
            vertexCount = std::stoi(line.substr(0, firstParen));
        }
        catch (...)
        {
            continue;
        }

        if (vertexCount <= 0)
        {
            continue;
        }

        Polygon polygon;
        bool isValid = true;

        // Parse points - find all (...) patterns
        size_t pos = firstParen;
        for (int i = 0; i < vertexCount; ++i)
        {
            size_t start = line.find('(', pos);
            size_t end = line.find(')', start);

            if (start == std::string::npos || end == std::string::npos)
            {
                isValid = false;
                break;
            }

            std::string pointStr = line.substr(start + 1, end - start - 1);
            size_t separatorPos = pointStr.find(';');

            if (separatorPos == std::string::npos)
            {
                isValid = false;
                break;
            }

            try
            {
                int xVal = std::stoi(pointStr.substr(0, separatorPos));
                int yVal = std::stoi(pointStr.substr(separatorPos + 1));
                polygon.points.push_back({xVal, yVal});
                pos = end + 1;
            }
            catch (...)
            {
                isValid = false;
                break;
            }
        }

        if (isValid && polygon.points.size() == static_cast<size_t>(vertexCount))
        {
            polygons.push_back(polygon);
        }
    }

    inputFile.close();

    PolygonManager manager(polygons);
    manager.processCommands();

    return 0;
}