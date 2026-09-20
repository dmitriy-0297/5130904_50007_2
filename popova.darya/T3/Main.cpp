#include "Polygon.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

static void readFile(std::ifstream& file,
    std::vector<Polygon>& polygons)
{
    std::string line;

    if (!std::getline(file, line))
        return;

    Polygon polygon;

    if (parsePolygon(line, polygon))
        polygons.push_back(std::move(polygon));

    readFile(file, polygons);
}

static void readCommands(std::vector<Polygon>& polygons)
{
    std::string command;

    if (!std::getline(std::cin, command))
        return;

    processCommand(command, polygons);

    readCommands(polygons);
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "File name is required\n";
        return 1;
    }

    std::ifstream file(argv[1]);

    if (!file)
    {
        std::cerr << "Cannot open file\n";
        return 1;
    }

    std::vector<Polygon> polygons;

    readFile(file, polygons);
    file.close();

    readCommands(polygons);

    return 0;
}