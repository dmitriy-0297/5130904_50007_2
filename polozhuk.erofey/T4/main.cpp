#include <iostream>
#include <iomanip>
#include <memory>
#include <vector>
#include "rectangle.h"
#include "rhombus.h"
#include "compositeshape.h"
int main() {
    std::cout << std::fixed << std::setprecision(2);
    {
        std::vector<std::unique_ptr<Shape>> shapes;
        shapes.push_back(std::make_unique<Rectangle>(Point(1.0, 1.0), Point(5.0, 4.0)));
        shapes.push_back(std::make_unique<Rhombus>(Point(0.0, 0.0), 6.0, 4.0));
        shapes.push_back(std::make_unique<Rectangle>(Point(-2.0, -2.0), Point(2.0, 2.0)));
        shapes.push_back(std::make_unique<Rhombus>(Point(3.0, 3.0), 4.0, 2.0));
        auto composite = std::make_unique<CompositeShape>();
        composite->addShape(std::make_unique<Rectangle>(Point(5.0, 5.0), Point(7.0, 7.0)));
        composite->addShape(std::make_unique<Rhombus>(Point(6.0, 6.0), 2.0, 2.0));
        shapes.push_back(std::move(composite));
        for (const auto& shape : shapes) {
            shape->print();
            std::cout << "\n";
        }
        for (auto& shape : shapes) {
            shape->scale(2.0);
        }
        for (const auto& shape : shapes) {
            shape->print();
            std::cout << "\n";
        }
    }
    {
        std::vector<Shape*> shapes2;
        shapes2.push_back(new Rectangle(Point(1.0, 1.0), Point(5.0, 4.0)));
        shapes2.push_back(new Rhombus(Point(0.0, 0.0), 6.0, 4.0));
        shapes2.push_back(new Rectangle(Point(-2.0, -2.0), Point(2.0, 2.0)));
        shapes2.push_back(new Rhombus(Point(3.0, 3.0), 4.0, 2.0));
        auto* composite2 = new CompositeShape();
        composite2->addShape(std::make_unique<Rectangle>(Point(5.0, 5.0), Point(7.0, 7.0)));
        composite2->addShape(std::make_unique<Rhombus>(Point(6.0, 6.0), 2.0, 2.0));
        shapes2.push_back(composite2);
        for (Shape* shape : shapes2) {
            shape->print();
            std::cout << "\n";
        }
        for (Shape* shape : shapes2) {
            shape->scale(2.0);
        }
        for (Shape* shape : shapes2) {
            shape->print();
            std::cout << "\n";
        }
        for (Shape* shape : shapes2) {
            delete shape;
        }
    }
    return 0;
}



