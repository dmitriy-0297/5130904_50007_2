#include <iostream>
#include <string>
#include <vector>
#include "Shape.h"

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    std::cerr << "Error: no input file\n";
    return 1;
  }

  std::vector<Polygon> polygons = read_polygons(argv[1]);

  process_commands(polygons);

  return 0;
}
