#include <iostream>
#include <vector>
#include <memory>
#include <iomanip>
#include "point.h"
#include "shape.h"
#include "rectangle.h"
#include "circle.h"
#include "composite_shape.h"

void printShapes(const std::vector<std::unique_ptr<Shape>>& shapes) {
    std::cout << std::fixed << std::setprecision(2);
    for (const auto& shape : shapes) {
        shape->print(std::cout, true);
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

int main() {
    std::vector<std::unique_ptr<Shape>> shapes;

    shapes.push_back(std::make_unique<Rectangle>(Point(0.0, 0.0), Point(4.0, 3.0)));
    shapes.push_back(std::make_unique<Circle>(Point(5.0, 5.0), 2.0));
    shapes.push_back(std::make_unique<Circle>(Point(10.0, 0.0), 1.5));
    shapes.push_back(std::make_unique<Rectangle>(Point(-5.0, -5.0), Point(-2.0, -1.0)));

    std::vector<std::unique_ptr<Shape>> compositeParts;
    compositeParts.push_back(std::make_unique<Rectangle>(Point(0.0, 0.0), Point(2.0, 2.0)));
    compositeParts.push_back(std::make_unique<Circle>(Point(3.0, 1.0), 1.0));
    shapes.push_back(std::make_unique<CompositeShape>(std::move(compositeParts)));

    std::cout << "Before scaling:" << std::endl;
    printShapes(shapes);

    for (auto& shape : shapes) {
        shape->scale(2.0);
    }

    std::cout << "After scaling (x2):" << std::endl;
    printShapes(shapes);

    return 0;
}
