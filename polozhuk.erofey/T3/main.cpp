#include <iostream>
#include <vector>
#include <fstream>
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
    std::istream& operator>>(std::istream& in, Polygon& poly) {
        int n = 0;
        if (!(in >> n)) {
            return in;
        }
        if (n < 3) {
            in.setstate(std::ios::failbit);
            return in;
        }
        std::vector<Point> temp_points(n);
        for (size_t i = 0; i < n; ++i) {
            if (!(in >> temp_points[i])) {
                return in;
            }
        }
        poly.points_ = std::move(temp_points);
        return in;
    }














































}


















































int main(int argc, char* argv[]) {

}