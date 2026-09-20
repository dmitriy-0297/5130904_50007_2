#include <iomanip>
#include <iostream>
#include <memory>
#include <vector>

#include "Rectangle.h"
#include "Trapezoid.h"
#include "CompositeShape.h"

void printShape(const Shape& shape)
{
    Point center = shape.getCenter();

    std::cout << "  ["
        << shape.getName()
        << ", ("
        << std::fixed << std::setprecision(2)
        << center.x << ", "
        << center.y << "), "
        << shape.getArea()
        << "]\n";
}

void printComposite(const CompositeShape& composite)
{
    Point center = composite.getCenter();

    std::cout << "  ["
        << composite.getName()
        << ", ("
        << std::fixed << std::setprecision(2)
        << center.x << ", "
        << center.y << "), "
        << composite.getArea()
        << ":\n";

    for (const auto& shape : composite.getShapes())
    {
        Point shapeCenter = shape->getCenter();

        std::cout << "    "
            << shape->getName()
            << ", ("
            << shapeCenter.x << ", "
            << shapeCenter.y << "), "
            << shape->getArea()
            << "]\n";
    }
}

void printAll(const std::vector<std::unique_ptr<Shape>>& shapes)
{
    for (const auto& shape : shapes)
    {
        if (const auto* composite =
            dynamic_cast<const CompositeShape*>(shape.get()))
        {
            printComposite(*composite);
        }
        else
        {
            printShape(*shape);
        }
    }
}

int main()
{
    std::vector<std::unique_ptr<Shape>> shapes;

    shapes.push_back(
        std::make_unique<Rectangle>(
            Point{ 0.0, 0.0 },
            Point{ 4.0, 3.0 }
        )
    );

    shapes.push_back(
        std::make_unique<Rectangle>(
            Point{ 5.0, 1.0 },
            Point{ 9.0, 5.0 }
        )
    );

    shapes.push_back(
        std::make_unique<Trapezoid>(
            Point{ 0.0, 5.0 },
            8.0,
            4.0,
            3.0
        )
    );

    shapes.push_back(
        std::make_unique<Trapezoid>(
            Point{ 6.0, 6.0 },
            6.0,
            3.0,
            4.0
        )
    );

    auto composite = std::make_unique<CompositeShape>();

    composite->add(
        std::make_unique<Rectangle>(
            Point{ 10.0, 0.0 },
            Point{ 14.0, 2.0 }
        )
    );

    composite->add(
        std::make_unique<Trapezoid>(
            Point{ 11.0, 3.0 },
            6.0,
            4.0,
            2.0
        )
    );

    shapes.push_back(std::move(composite));

    std::cout << std::fixed << std::setprecision(2);

    std::cout << "BEFORE SCALE:\n";
    printAll(shapes);

    for (auto& shape : shapes)
    {
        shape->scale(2.0);
    }

    std::cout << "\nAFTER SCALE x2:\n";
    printAll(shapes);

    return 0;
}