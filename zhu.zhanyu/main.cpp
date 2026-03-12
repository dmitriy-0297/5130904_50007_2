#include <iostream>
#include <iomanip>
#include <memory>
#include <vector>
#include <new>
#include "Rectangle.h"
#include "Square.h"
#include "CompositeShape.h"

void printShape(const Shape& shape) {
    Point center = shape.getCenter();
    std::cout << "[" << shape.getName() << ", ("
        << std::fixed << std::setprecision(2)
        << center.x << ", " << center.y << "), "
        << shape.getArea() << "]";
}

void printComposite(const CompositeShape& composite) {
    Point center = composite.getCenter();
    std::cout << "[COMPOSITE, (" << std::fixed << std::setprecision(2)
        << center.x << ", " << center.y << "), "
        << composite.getArea() << ":\n";
    for (size_t i = 0; i < composite.getShapeCount(); ++i) {
        std::cout << "  ";
        printShape(*composite.getShape(i));
        if (i < composite.getShapeCount() - 1) {
            std::cout << ",";
        }
        std::cout << "\n";
    }
    std::cout << "]";
}

int main() {
    try {
        std::cout << std::fixed << std::setprecision(2);

        std::vector<std::unique_ptr<Shape>> shapes;

        shapes.push_back(std::make_unique<Rectangle>(Point{ 1, 1 }, Point{ 5, 4 }));
        shapes.push_back(std::make_unique<Square>(Point{ 2, 2 }, 3));
        shapes.push_back(std::make_unique<Rectangle>(Point{ 0, 0 }, Point{ 3, 2 }));
        shapes.push_back(std::make_unique<Square>(Point{ -1, -1 }, 2));

        auto composite = std::make_unique<CompositeShape>();
        composite->addShape(std::make_unique<Rectangle>(Point{ 1, 1 }, Point{ 4, 3 }));
        composite->addShape(std::make_unique<Square>(Point{ 2, 2 }, 2));
        shapes.push_back(std::move(composite));

        std::cout << "Before scaling:\n";
        for (const auto& shape : shapes) {
            if (shape->getName() == "COMPOSITE") {
                printComposite(static_cast<const CompositeShape&>(*shape));
            }
            else {
                printShape(*shape);
            }
            std::cout << "\n";
        }

        std::cout << "\nAfter scaling by 2:\n";
        for (auto& shape : shapes) {
            shape->scale(2.0);
        }

        for (const auto& shape : shapes) {
            if (shape->getName() == "COMPOSITE") {
                printComposite(static_cast<const CompositeShape&>(*shape));
            }
            else {
                printShape(*shape);
            }
            std::cout << "\n";
        }
    }
    catch (const std::bad_alloc& e) {
        std::cerr << "Memory allocation failed: " << e.what() << std::endl;
        return 1;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
