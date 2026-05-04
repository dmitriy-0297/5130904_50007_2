#ifndef DATA_STRUCT_H
#define DATA_STRUCT_H

#include <vector>
#include <string>
#include <iostream>

struct Point {
  int x, y; //
};

struct Polygon {
  std::vector< Point > points; //
};

class PolygonManager {
public: //
  explicit PolygonManager(const std::vector< Polygon >& polygons);
  void processCommands();

private: //
  std::vector< Polygon > polygons_; //

  void printArea(std::istream& in) const;
  void printMax(std::istream& in) const;
  void printMin(std::istream& in) const;
  void printCount(std::istream& in) const;

  // Команды Варианта №2
  void printLessArea(std::istream& in) const;
  void printMaxSeq(std::istream& in) const;
};

// Вспомогательный функционал для работы с потоками
std::istream& operator>>(std::istream& in, Point& dest);
std::istream& operator>>(std::istream& in, Polygon& dest);

#endif