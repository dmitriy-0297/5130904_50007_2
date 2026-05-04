#include "DataStruct.h"

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
        std::stringstream ss(line);
        int vertexCount = 0;
        if (!(ss >> vertexCount))
        {
            continue;
        }

        Polygon polygon;
        bool isValid = true;
        for (int i = 0; i < vertexCount; ++i)
        {
            std::string pointStr;
            if (!(ss >> pointStr))
            {
                isValid = false;
                break;
            }

            if (pointStr.front() == '(' && pointStr.back() == ')')
            {
                pointStr = pointStr.substr(1, pointStr.length() - 2);
            }

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
