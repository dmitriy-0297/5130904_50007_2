#ifndef SQUARE_H
#define SQUARE_H
#include"Shape.h"
#include"Point.h"
class Square :public Shape {
private:
	Point leftBottom_;
	double size_;
public:
	Square(Point leftBottom, double size);
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
