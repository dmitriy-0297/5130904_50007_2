#include <iostream>
#include <iomanip>
#include <memory>
#include "Circle.h"
#include "Rectangle.h"
#include "CompositeShape.h"

int main()
{
  std::cout << std::fixed << std::setprecision(2);

  try
  {
    CompositeShape composite;

    composite.addShape(std::make_unique<Rectangle>(Point{0.0, 0.0}, Point{10.0, 10.0}));
    composite.addShape(std::make_unique<Circle>(Point{20.0, 20.0}, 5.0));

    std::cout << "Initial state:\n";
    std::cout << "Total Area: " << composite.getArea() << "\n";
    Point c = composite.getCenter();
    std::cout << "Center: (" << c.x_ << ", " << c.y_ << ")\n\n";

    composite.move({-5.0, -5.0});
    std::cout << "After move to (-5, -5):\n";
    Point nc = composite.getCenter();
    std::cout << "New Center: (" << nc.x_ << ", " << nc.y_ << ")\n\n";

    double areaBefore = composite.getArea();
    composite.scale(2.0);
    double areaAfter = composite.getArea();

    std::cout << "After scale x2.0:\n";
    std::cout << "New Area: " << areaAfter << "\n";
    std::cout << "Area Ratio: " << areaAfter / areaBefore << " (expected 4.00)\n\n";

    std::cout << "Testing exceptions:\n";
    try
    {
      composite.scale(-1.0);
    }
    catch (const std::invalid_argument& e)
    {
      std::cout << "Error caught: " << e.what() << "\n";
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << "Critical error: " << e.what() << "\n";
    return 1;
  }

  return 0;
}ы