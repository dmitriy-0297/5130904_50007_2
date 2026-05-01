#include <algorithm>
#include <iostream>
#include <vector>
#include <fstream>
#include <iterator>
#include <numeric>
#include <limits>
#include <iomanip>
#include <string>
#include <cmath>
#include <functional>
#include <map>

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

    bool isEven(const polozhuk::Polygon& p) {
    return (p.points_.size() % 2 == 0);
    }
    double doAccumulateEvenArea(double res, const Polygon& p){
        if (isEven(p)){
            res += getArea(p);
        }
        return res;
    }
    bool isOdd(const Polygon& p)
    {
        return !(p.points_.size() % 2 == 0);
    }
    void getAreaCmd(const std::vector<Polygon>& polygons, std::istream& in, std::ostream& out) {
        std::string line;
        in >> line;
        if (line == "EVEN") {
            double count = std::accumulate(polygons.cbegin(), polygons.cend(), 0.0,
                [](double insum, const polozhuk::Polygon& p) {
                    if (isEven(p)) {
                        return insum + polozhuk::getArea(p);
                    }
                     return insum;
                }
            );
            out << count << "\n";
        }
        else if (line == "ODD") {
            double count = std::accumulate(polygons.cbegin(), polygons.cend(), 0.0,
                [](double insum, const polozhuk::Polygon& p) {
                    if (!isEven(p)) {
                        return insum + polozhuk::getArea(p);
                    }
                     return insum;
                }
            );
            out << count << "\n";
        }
        else if (line == "MEAN") {
            if (polygons.empty()) {
                throw std::invalid_argument("<INVALID COMMAND>");
            }
            double bigSum = std::accumulate(polygons.cbegin(), polygons.cend(), 0.0,
                [](double sum, const polozhuk::Polygon& p) {
                    return sum + polozhuk::getArea(p);
                }
            );
            out << bigSum / polygons.size() << "\n";
        }
        else {
            size_t versh =  0;
            try {
                versh = std::stoull(line);
            }
            catch (...) {
                throw std::invalid_argument("<INVALID COMMAND>");
            }
            if (versh < 3) {
                throw std::invalid_argument("<INVALID COMMAND>");
            }
            double count = std::accumulate(polygons.cbegin(), polygons.cend(), 0.0,
                [versh](double sum, const polozhuk::Polygon& p) {
                    if (p.points_.size() ==  versh) {
                        sum += polozhuk::getArea(p);
                    }
                    return sum;
                });
            out << count << "\n";
        }
    }


    double accumulateMaxArea(double res, const Polygon& p){
        return std::max(res, getArea(p));
    }
    double accumulateMinArea(double res, const Polygon& p){
        return std::min(res, getArea(p));
    }
    size_t accumulateMinVertexes(size_t size, const Polygon& p){
        return std::min(size, p.points_.size());
    }
    size_t accumulateMaxVertexes(size_t size, const Polygon& p){
        return std::max(size, p.points_.size());

    }
    void getMinCmd(const std::vector<Polygon>& polygons, std::istream& in, std::ostream& out) {
        if (polygons.empty()) {
            throw std::invalid_argument("<INVALID COMMAND>");
        }
        out << std::setprecision(1) << std::fixed;
        std::string line;
        in >> line;
        using namespace std::placeholders;

        if (line == "AREA") {
            double maxDouble = std::numeric_limits< double >::max();
            out << std::accumulate(polygons.cbegin(), polygons.cend(), maxDouble, accumulateMinArea) << '\n';
        }
        else if (line == "VERTEXES"){
            size_t maxSize = std::numeric_limits< size_t >::max();
            out << std::accumulate(polygons.cbegin(), polygons.cend(), maxSize, accumulateMinVertexes) << '\n';
        }
        else
        {
            throw std::invalid_argument("<INVALID COMMAND>");
        }
    }
    void getMaxCmd(const std::vector<Polygon>& polygons, std::istream& in, std::ostream& out) {
        if (polygons.empty()) {
            throw std::invalid_argument("<INVALID COMMAND>");
        }
        out << std::setprecision(1) << std::fixed;
        std::string line;
        in >> line;
        using namespace std::placeholders;
        if (line == "AREA") {
            out << std::accumulate(polygons.cbegin(), polygons.cend(), 0.0, accumulateMaxArea) << '\n';
        }
        else if (line == "VERTEXES")
        {
            out << std::accumulate(polygons.cbegin(), polygons.cend(), 0ull, accumulateMaxVertexes) << '\n';
        }
        else
        {
            throw std::invalid_argument("<INVALID COMMAND>");
        }
    }

    void getCountCmd(const std::vector<Polygon>& polygons, std::istream& in, std::ostream& out) {
        out << std::setprecision(1) << std::fixed;
        std::string line;
        in >> line;
        using namespace std::placeholders;
        if (line == "EVEN") {
            size_t count = std::count_if(polygons.cbegin(), polygons.cend(), isEven);
            out << count << "\n";
        }
        else if (line == "ODD") {
            size_t count = std::count_if(polygons.cbegin(), polygons.cend(), isOdd);
            out << count << "\n";
        }
        else {
            size_t versh = 0;
            try {
                versh = std::stoull(line);
            }
            catch (...) {
                throw std::invalid_argument("<INVALID COMMAND>");
            }
            if (versh < 3) {
                throw std::invalid_argument("<INVALID COMMAND>");
            }
            size_t getCount = std::accumulate(polygons.cbegin(), polygons.cend(), 0,
                [versh](size_t count, const polozhuk::Polygon& p) {
                    if (p.points_.size() == versh) {
                        ++count;
                    }
                    return count;
                });
            out << getCount << "\n";
        }
    }
    void getMaxSeqCmd(const std::vector<polozhuk::Polygon>& polygons, std::istream& in, std::ostream& out) {
        Polygon cmd;
        if (!(in >> cmd) || cmd.points_.size() < 3) {
            throw std::invalid_argument("<INVALID COMMAND>");
        }
        char c;
        while (in.get(c) && c != '\n') {
            if (!std::isspace(c)) {
                throw std::invalid_argument("<INVALID COMMAND>");
            }
        }
        if (polygons.empty()) {
            out << "0\n";
            return;
        }
        size_t cur_seq = 0;
        auto accum = std::accumulate(polygons.cbegin(), polygons.cend(), 0ull,
            [&cmd, &cur_seq](size_t max_seq, const polozhuk::Polygon& p) {
                if (p.points_ == cmd.points_) {
                    ++cur_seq;
                    if (cur_seq > max_seq) {
                        max_seq = cur_seq;
                    }
                }
                else {
                    cur_seq = 0;
                }
                return max_seq;
            });
        out << accum << "\n";
    }

    std::ostream& operator<<(std::ostream& out, const Point& pt) {
        out << "(" << pt.x_ << ";" << pt.y_ << ")";
        return out;
    }
    std::ostream& operator<<(std::ostream& out, const Polygon& poly) {
        out << poly.points_.size() << " ";
        std::copy(
            poly.points_.cbegin(),
            poly.points_.cend(),
            std::ostream_iterator<Point>(out, " ")
        );
        return out;
    }
    void repeat(std::vector<Polygon>& polygons, std::istream& in, std::ostream& out) {
        Polygon target;
        if (!(in >> target) || target.points_.size() < 3) {
            throw std::invalid_argument("<INVALID COMMAND>");
        }
        char c;
        while (in.get(c) && c != '\n') {
            if (!std::isspace(c)) {
                throw std::invalid_argument("<INVALID COMMAND>");
            }
        }
        size_t count = std::count(polygons.cbegin(), polygons.cend(), target);
        if (count > 0) {
            std::vector<Polygon> new_polygons;
            new_polygons.reserve(polygons.size() + count);
            std::for_each(polygons.cbegin(), polygons.cend(),
                [&new_polygons, &target](const Polygon& p) {
                    new_polygons.push_back(p);
                    if (p == target) {
                        new_polygons.push_back(p);
                    }
                }
            );
            polygons = std::move(new_polygons);
        }
        out << count << "\n";
    }
};

namespace mymap {
    std::map<std::string,std::
    function<void(std::vector<polozhuk::Polygon>&, std::istream&, std::ostream&)>> mapCmd{
        {"AREA", polozhuk::getAreaCmd},
        {"ECHO", polozhuk::repeat},
        {"MAXSEQ", polozhuk::getMaxSeqCmd},
        {"COUNT", polozhuk::getCountCmd},
        {"MIN", polozhuk::getMinCmd},
        {"MAX", polozhuk::getMaxCmd}
    };
}
int main(int argc, char* argv[]) {

    if (argc < 2) {
        std::cerr << "Error: No input file provided.\n";
        return 1;
    }
    std::ifstream inputFile(argv[1]);
    if (!inputFile.is_open()) {
        std::cerr << "Error: Could not open file\n";
        return 1;
    }
    std::cout << std::setprecision(1) << std::fixed;
    std::vector<polozhuk::Polygon> polygon;
    polozhuk::Polygon line;
    while (inputFile>>line) {
        polygon.push_back(line);
    }
    if (!inputFile.eof() && inputFile.fail()) {
        std::cerr << "Error: File format is invalid.\n";
        return 1;
    }
    std::string cmd ;
    while (std::cin >> cmd) {
        try {
            mymap::mapCmd.at(cmd)(polygon, std::cin, std::cout);

        }
        catch (const std::out_of_range&) {
            std::cout << "<INVALID COMMAND>\n";
            polozhuk::dvornik(std::cin);
        }
        catch (const std::invalid_argument& e) {
            std::cout << e.what() << "\n";
            polozhuk::dvornik(std::cin);
        }

    }
    return 0;
}
