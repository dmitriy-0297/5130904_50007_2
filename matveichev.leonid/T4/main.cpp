#include <iostream>
#include <vector>
#include <memory>
#include <iomanip>
#include "shape.h"
#include "rectangle.h"
#include "ellipse.h"
#include "composite_shape.h"

int main() {
    std::vector<std::unique_ptr<Shape>> shapes;

    shapes.push_back(std::make_unique<Rectangle>(Point(0.0, 0.0), Point(4.0, 2.0)));
    shapes.push_back(std::make_unique<Ellipse>(Point(10.0, 10.0), 5.0, 3.0));

    auto composite = std::make_unique<CompositeShape>();
    composite->addShape(std::make_unique<Rectangle>(Point(0.0, 0.0), Point(2.0, 2.0)));
    composite->addShape(std::make_unique<Ellipse>(Point(5.0, 5.0), 3.0, 2.0));
    composite->addShape(std::make_unique<Rectangle>(Point(10.0, 0.0), Point(12.0, 4.0)));
    shapes.push_back(std::move(composite));

    shapes.push_back(std::make_unique<Rectangle>(Point(-5.0, -5.0), Point(-1.0, -1.0)));
    shapes.push_back(std::make_unique<Ellipse>(Point(20.0, 0.0), 4.0, 2.0));

    std::cout << std::fixed << std::setprecision(2);

    for (const auto& shape : shapes) {
        shape->print(std::cout);
        std::cout << std::endl;
    }

    std::cerr << "Error: scaling not implemented" << std::endl;
    return 2;
}