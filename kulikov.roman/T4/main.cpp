#include <iostream>
#include <iomanip>
#include <memory>
#include <vector>
#include "rectangle.h"
#include "trapez.h"
#include "compositeshape.h"

void printShape(const Shape* shape) {
    shape->print();
    std::cout << std::endl;
}

int main() {
    std::vector<std::unique_ptr<Shape>> shapes;

    shapes.push_back(std::make_unique<Rectangle>(Point(-1, 1), Point(5, 3)));
    shapes.push_back(std::make_unique<Rectangle>(Point(0, 0), Point(4, 3)));
    shapes.push_back(std::make_unique<Trapezoid>(Point(-1, 1), 6, 3, 2));
    shapes.push_back(std::make_unique<Rectangle>(Point(2, 0), Point(6, 4)));

    auto composite = std::make_unique<CompositeShape>();
    composite->addShape(std::make_unique<Rectangle>(Point(0, 0), Point(2, 2)));
    composite->addShape(std::make_unique<Rectangle>(Point(3, 1), Point(5, 3)));
    shapes.push_back(std::move(composite));

    for (size_t i = 0; i < shapes.size(); i++) {
        std::cout << "Figure " << (i + 1) << ": ";
        printShape(shapes[i].get());
    }

    std::cerr << "Error: scaling is required" << std::endl;
    
    return 2;
}

// Remove trailing whitespace - Wed Mar 11 01:37:48 RTZ 2026
