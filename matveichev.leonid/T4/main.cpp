#include <iostream>
#include <vector>
#include <memory>
#include <iomanip>
#include <stdexcept>
#include "shape.h"
#include "rectangle.h"
#include "ellipse.h"
#include "composite_shape.h"

int main() {
    try {
        std::vector<std::unique_ptr<Shape>> shapes;

        shapes.push_back(std::make_unique<Rectangle>(
            Point(0.0, 0.0), Point(4.0, 2.0)));
        shapes.push_back(std::make_unique<Ellipse>(
            Point(10.0, 10.0), 5.0, 3.0));

        auto composite = std::make_unique<CompositeShape>();
        composite->addShape(std::make_unique<Rectangle>(
            Point(0.0, 0.0), Point(2.0, 2.0)));
        composite->addShape(std::make_unique<Ellipse>(
            Point(5.0, 5.0), 3.0, 2.0));
        composite->addShape(std::make_unique<Rectangle>(
            Point(10.0, 0.0), Point(12.0, 4.0)));
        shapes.push_back(std::move(composite));

        shapes.push_back(std::make_unique<Rectangle>(
            Point(-5.0, -5.0), Point(-1.0, -1.0)));
        shapes.push_back(std::make_unique<Ellipse>(
            Point(20.0, 0.0), 4.0, 2.0));

        if (shapes.empty()) {
            throw std::runtime_error("No shapes created");
        }

        if (shapes.size() < 5) {
            throw std::runtime_error("Less than 5 shapes: " +
                                     std::to_string(shapes.size()));
        }

        bool hasComposite = false;
        for (const auto& shape : shapes) {
            if (!shape) {
                throw std::runtime_error("Null pointer in container");
            }
            if (shape->getName() == "COMPOSITE") {
                hasComposite = true;
            }
        }
        if (!hasComposite) {
            throw std::runtime_error("No composite shape found");
        }

        std::cout << std::fixed << std::setprecision(2);

        std::cout << "=== Figures BEFORE scaling (x2) ===\n";
        for (const auto& shape : shapes) {
            shape->print(std::cout);
            std::cout << '\n';
        }

        std::cout << "\n=== Scaling all figures by 2.0 ===\n\n";

        for (auto& shape : shapes) {
            shape->scale(2.0);
        }

        std::cout << "=== Figures AFTER scaling (x2) ===\n";
        for (const auto& shape : shapes) {
            shape->print(std::cout);
            std::cout << '\n';
        }

        return 0;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}
