#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "Polygon.h"

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Error: No file specified\n";
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file.is_open())
    {
        std::cerr << "Error: Cannot open file\n";
        return 1;
    }

    std::vector< Polygon > polygons;
    std::string line;

    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        Polygon tempPolygon;
        if (iss >> tempPolygon)
        {
            std::string garbage;
            if (!(iss >> garbage))
            {
                polygons.push_back(tempPolygon);
            }
        }
    }

    while (std::getline(std::cin, line))
    {
        std::istringstream iss(line);
        std::string command;
        if (!(iss >> command))
        {
            continue;
        }

        if (command == "AREA")
        {
            processArea(polygons, iss);
        }
        else if (command == "MAX")
        {
            processMax(polygons, iss);
        }
        else if (command == "MIN")
        {
            processMin(polygons, iss);
        }
        else if (command == "COUNT")
        {
            processCount(polygons, iss);
        }
        else if (command == "RECTS")
        {
            processRects(polygons);
        }
        else if (command == "MAXSEQ")
        {
            processMaxSeq(polygons, iss);
        }
        else
        {
            printInvalidCommand();
        }
    }

    return 0;
}

