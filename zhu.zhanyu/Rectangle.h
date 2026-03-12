#ifndef RECTANGLE_H
#define RECTANGLE_H
#include"Point.h"
#include"Shape.h"
class Rectangle :public Shape {
private:
	Point leftBottom_;
	Point rightTop_;
public:
	Rectangle(Point leftBottom, Point rightTop);
	double getArea()const override;
	Point getCenter()const override;
	void move(double dx, double dy)override;
	void scale(double factor)override;
	std::string getName()const override;
	double getMinX()const override;
	double getMinY()const override;
	double getMaxX()const override;
	double getMaxY()const override;
};
#endif
