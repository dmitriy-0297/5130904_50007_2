#include <algorithm>
#include <iostream>
#include <vector>
#include <fstream>
#include <iterator>
#include <numeric>
#include <limits>

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

    bool operator==(const Point& lhs, const Point& rhs) {
        if (lhs.x_ != rhs.x_ || lhs.y_ != rhs.y_) {
            return false;
        }
        return true;
    }
    bool operator!=(const Point& lhs, const Point& rhs) {
        return !(lhs == rhs);
    }
    bool operator<(const Point& lhs, const Point& rhs) { return lhs.x_ < rhs.x_ || lhs.y_ < rhs.y_; }
    bool operator>(const Point& lhs, const Point& rhs) { return rhs < lhs; }
    bool operator<(const Polygon& lhs, const Polygon& rhs) {
        return lhs.points_ < rhs.points_;
    }
    bool operator>(const Polygon& lhs, const Polygon& rhs) {
        return rhs < lhs;
    }
    bool operator==(const Polygon& lhs, const Polygon& rhs) {
        if (lhs.points_ != rhs.points_) {
            return false;
        }
        return true;
    }
    bool operator!=(const Polygon& lhs, const Polygon& rhs) {
        return !(lhs == rhs);
    }


    double getArea(const Polygon& polygon) {
        std::vector<double> rigtLeft;
        std::vector<double> leftRight;
        rigtLeft.reserve(polygon.points_.size());
        leftRight.reserve(polygon.points_.size());
        std::transform(
        polygon.points_.cbegin(),
        polygon.points_.cend() - 1,
        polygon.points_.cbegin()+1,
        std::back_inserter(rigtLeft),
        [](const polozhuk::Point& p1, const polozhuk::Point& p2) {return p1.x_ * p2.y_;}
        );
        std::transform(
        polygon.points_.cbegin(),
        polygon.points_.cend() - 1,
        polygon.points_.cbegin()+1,
        std::back_inserter(leftRight),
        [](const polozhuk::Point& p1, const polozhuk::Point& p2) {return p1.y_ * p2.x_;}
        );
        rigtLeft.push_back(polygon.points_[polygon.points_.size() - 1].x_ * polygon.points_[0].y_);
        leftRight.push_back(polygon.points_[polygon.points_.size() - 1].y_ * polygon.points_[0].x_);
        double sRL = std::accumulate(std::begin(rigtLeft), std::end(rigtLeft), 0.0);
        double sLR = std::accumulate(std::begin(leftRight), std::end(leftRight), 0.0);
        double S = std::abs(sLR - sRL) * 0.5;
        return S ;
    }

    void dvornik(std::istream& in) {
        in.clear();
        in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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





