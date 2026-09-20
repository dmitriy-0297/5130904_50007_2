#include <iostream>
#include <vector>
#include <memory>
#include <iomanip>
#include <string>

#include "rectangle.h"
#include "ring.h"
#include "composite_shape.h"

void printSimple(const Shape& shape) {
    Point c = shape.getCenter();

    std::cout << std::fixed << std::setprecision(2);

    std::cout << "[" << shape.getName() << ", ("
              << c.x << ", " << c.y << "), "
              << shape.getArea() << "]";
}

void printComposite(const CompositeShape& composite) {
    Point c = composite.getCenter();

    std::cout << std::fixed << std::setprecision(2);

    std::cout << "[" << composite.getName() << ", ("
              << c.x << ", " << c.y << "), "
              << composite.getArea() << ":\n";

    const auto& children = composite.getShapes();

    for (size_t i = 0; i < children.size(); ++i) {
        std::cout << "  ";

        printSimple(*children[i]);

        if (i + 1 < children.size()) {
            std::cout << ",";
        }

        std::cout << "\n";
    }

    std::cout << "]";
}

int main() {
    try {
        std::vector<std::unique_ptr<Shape>> shapes;

        shapes.push_back(std::make_unique<Rectangle>(
            Point{0.0, 0.0},
            Point{4.0, 2.0}));

        shapes.push_back(std::make_unique<Ring>(
            Point{10.0, 5.0},
            4.0,
            2.0));

        shapes.push_back(std::make_unique<Rectangle>(
            Point{-10.0, -10.0},
            Point{-5.0, -8.0}));

        shapes.push_back(std::make_unique<Ring>(
            Point{0.0, 10.0},
            3.0,
            1.5));

        auto composite = std::make_unique<CompositeShape>();

        composite->add(std::make_unique<Rectangle>(
            Point{1.0, 1.0},
            Point{3.0, 3.0}));

        composite->add(std::make_unique<Ring>(
            Point{6.0, 2.0},
            2.0,
            1.0));

        std::cout << "--- BEFORE SCALING ---\n";

        for (const auto& s : shapes) {
            printSimple(*s);
            std::cout << "\n";
        }

        printComposite(*composite);
        std::cout << "\n\n";

        const double SCALE_FACTOR = 2.0;

        for (auto& s : shapes) {
            s->scale(SCALE_FACTOR);
        }

        composite->scale(SCALE_FACTOR);

        std::cout << "--- AFTER SCALING (x2.00) ---\n";

        for (const auto& s : shapes) {
            printSimple(*s);
            std::cout << "\n";
        }

        printComposite(*composite);
        std::cout << "\n";
    }
    catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
