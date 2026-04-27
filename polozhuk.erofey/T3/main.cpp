#include <algorithm>
#include <iostream>
#include <vector>
#include <fstream>
#include <iterator>

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


    std::istream& operator>>(std::istream& in, Polygon& p) {
        std::istream::sentry sentry(in);
        if (!sentry) {
            return in;
        }
        size_t countPoints = 0;
        if (!(in >> countPoints) || countPoints < 3) {
            in.setstate(std::ios::failbit);
            return in;
        }
        std::vector<Point> tmp;
        std::copy_n((std::istream_iterator<Point>{in}), countPoints, std::back_inserter(tmp));

        if (in) {
            p.points_ = std::move(tmp);
        }

        return in;
    }














































}


















































int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Error: No input file provided.\n";
        return 1;
    }
    std::string filename = argv[1];
    std::ifstream inputFile(filename);

    if (!inputFile.is_open()) {
        std::cerr << "Error: Could not open file " << filename << "\n";
        return 1;
    }

    std::vector<polozhuk::Polygon> polygons;
    polozhuk::Polygon tempPoly;
    while (inputFile >> tempPoly) {
        polygons.push_back(tempPoly);
    }
    std::cout << "Successfully read " << polygons.size() << " polygons.\n";
    return 0;
}





