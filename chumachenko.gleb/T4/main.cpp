#include <iostream>
#include <vector>
#include <memory>
#include <iomanip>
#include <string>

#include "rectangle.h"
#include "isosceles_trapezoid.h"
#include "composite_shape.h"

void printShapeDetails(const Shape& shape) {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "[" << shape.getName()
        << ", (" << shape.getCenter().x << ", " << shape.getCenter().y << "), "
        << shape.getArea();

    if (shape.getName() == "COMPOSITE") {
        const auto& composite = static_cast<const CompositeShape&>(shape);
        const auto& children = composite.getShapes();

        std::cout << ": \n";
        for (size_t i = 0; i < children.size(); ++i) {
            const auto& child = children[i];
            std::cout << child->getName() << ", ("
                << child->getCenter().x << ", " << child->getCenter().y << "), "
                << child->getArea();

            if (i < children.size() - 1) {
                std::cout << ", \n";
            }
        }
    }


    std::cout << (shape.getName() == "COMPOSITE" ? "\n]" : "]");

}

void displayShapes(const std::string& header, const std::vector<std::unique_ptr<Shape>>& container) {
    std::cout << "--- " << header << " ---\n";
    for (const auto& shapePtr : container) {
        if (shapePtr) {
            printShapeDetails(*shapePtr);
            std::cout << "\n";
        }
    }
    std::cout << std::endl;
}

int main() {
    try {
        std::vector<std::unique_ptr<Shape>> shapes;

        shapes.push_back(std::make_unique<Rectangle>(Point{ 0.0, 0.0 }, Point{ 4.0, 2.0 }));
        shapes.push_back(std::make_unique<IsoscelesTrapezoid>(Point{ 10.0, 0.0 }, 6.0, 2.0, 4.0));
        shapes.push_back(std::make_unique<Rectangle>(Point{ -10.0, -10.0 }, Point{ -5.0, -8.0 }));
        shapes.push_back(std::make_unique<IsoscelesTrapezoid>(Point{ 0.0, 10.0 }, 8.0, 4.0, 2.0));

        auto composite = std::make_unique<CompositeShape>();
        composite->add(std::make_unique<Rectangle>(Point{ 1.0, 1.0 }, Point{ 3.0, 3.0 }));
        composite->add(std::make_unique<IsoscelesTrapezoid>(Point{ 5.0, 5.0 }, 4.0, 2.0, 2.0));

        shapes.push_back(std::move(composite));

        displayShapes("BEFORE SCALING", shapes);

        const double scaleFactor = 2.0;
        for (auto& shapePtr : shapes) {
            shapePtr->scale(scaleFactor);
        }

        std::string checkin;
        if (!(std::cin >> checkin))
        {
            std::cerr << "ERROR: No input\n";
            return 1;
        }

        displayShapes("AFTER SCALING (x2.00)", shapes);

    }
    catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

