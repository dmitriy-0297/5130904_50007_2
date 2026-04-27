#include <iostream>
#include <vector>
namespace polozhuk{
    struct Point {
        int x_;
        int y_;
    };

    struct Polygon {
        std::vector<Point> points_;
    };
    std::istream& operator>>(std::istream& in, Point& p) {
        char c;
        if (!(in >> c) || c != '(') {
            in.setstate(std::ios::failbit);
            return in;
        }

        if (!(in >> p.x_)) {
            return in;
        }

        if (!(in >> c) || c != ';') {
            in.setstate(std::ios::failbit);
            return in;
        }
        if ( !(in >> p.y_)) {
            return in;
        }
        if ( !(in >> c) || c != ')') {
            in.setstate(std::ios::failbit);
            return in;
        }

        return in;
    }













































}


















































int main() {

}