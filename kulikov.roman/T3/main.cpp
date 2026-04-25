#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <iomanip>
#include <cmath>
#include <cctype>

struct Point {
    int x;
    int y;
};

struct Polygon {
    std::vector<Point> points;
};

std::istream& operator>>(std::istream& in, Point& p) {
    char c;
    if (!(in >> c) || c != '(') {
        in.setstate(std::ios::failbit);
        return in;
    }
    if (!(in >> p.x)) {
        return in;
    }
    if (!(in >> c) || c != ';') {
        in.setstate(std::ios::failbit);
        return in;
    }
    if (!(in >> p.y)) {
        return in;
    }
    if (!(in >> c) || c != ')') {
        in.setstate(std::ios::failbit);
        return in;
    }
    return in;
}

std::istream& operator>>(std::istream& in, Polygon& poly) {
    int n = 0;
    if (!(in >> n) || n < 3) {
        in.setstate(std::ios::failbit);
        return in;
    }
    std::vector<Point> pts(n);
    for (int i = 0; i < n; ++i) {
        if (!(in >> pts[i])) {
            in.setstate(std::ios::failbit);
            return in;
        }
    }
    poly.points = std::move(pts);
    return in;
}

double calcArea(const Polygon& poly) {
    const auto& pts = poly.points;
    int n = static_cast<int>(pts.size());
    long long sum = 0;
    for (int i = 0; i < n; ++i) {
        int j = (i + 1) % n;
        long long term1 = static_cast<long long>(pts[i].x) * pts[j].y;
        long long term2 = static_cast<long long>(pts[j].x) * pts[i].y;
        sum += term1 - term2;
    }
    return std::abs(sum) / 2.0;
}

bool isSameShape(const Polygon& a, const Polygon& b) {
    if (a.points.size() != b.points.size()) {
        return false;
    }
    int n = static_cast<int>(a.points.size());
    if (n == 0) {
        return true;
    }

    for (int start = 0; start < n; ++start) {
        int dx = b.points[start].x - a.points[0].x;
        int dy = b.points[start].y - a.points[0].y;
        bool match = true;
        for (int i = 0; i < n; ++i) {
            int bi = (start + i) % n;
            if (a.points[i].x + dx != b.points[bi].x ||
                a.points[i].y + dy != b.points[bi].y) {
                match = false;
                break;
            }
        }
        if (match) {
            return true;
        }
    }

    for (int start = 0; start < n; ++start) {
        int dx = b.points[start].x - a.points[0].x;
        int dy = b.points[start].y - a.points[0].y;
        bool match = true;
        for (int i = 0; i < n; ++i) {
            int bi = (start - i + n) % n;
            if (a.points[i].x + dx != b.points[bi].x ||
                a.points[i].y + dy != b.points[bi].y) {
                match = false;
                break;
            }
        }
        if (match) {
            return true;
        }
    }

    return false;
}

void doArea(const std::vector<Polygon>& polys, const std::string& arg) {
    if (polys.empty() && arg == "MEAN") {
        std::cout << "<INVALID COMMAND>\n";
        return;
    }

    if (arg == "EVEN") {
        double sum = std::accumulate(
            polys.begin(), polys.end(), 0.0,
            [](double acc, const Polygon& p) {
                return (p.points.size() % 2 == 0) ? acc + calcArea(p) : acc;
            });
        std::cout << std::fixed << std::setprecision(1) << sum << "\n";
    } else if (arg == "ODD") {
        double sum = std::accumulate(
            polys.begin(), polys.end(), 0.0,
            [](double acc, const Polygon& p) {
                return (p.points.size() % 2 != 0) ? acc + calcArea(p) : acc;
            });
        std::cout << std::fixed << std::setprecision(1) << sum << "\n";
    } else if (arg == "MEAN") {
        double sum = std::accumulate(
            polys.begin(), polys.end(), 0.0,
            [](double acc, const Polygon& p) {
                return acc + calcArea(p);
            });
        std::cout << std::fixed << std::setprecision(1)
                  << sum / polys.size() << "\n";
    } else {
        int n = 0;
        try {
            n = std::stoi(arg);
        } catch (...) {
            std::cout << "<INVALID COMMAND>\n";
            return;
        }
        if (n < 3) {
            std::cout << "<INVALID COMMAND>\n";
            return;
        }
        double sum = std::accumulate(
            polys.begin(), polys.end(), 0.0,
            [n](double acc, const Polygon& p) {
                return (static_cast<int>(p.points.size()) == n)
                           ? acc + calcArea(p)
                           : acc;
            });
        std::cout << std::fixed << std::setprecision(1) << sum << "\n";
    }
}

void doMax(const std::vector<Polygon>& polys, const std::string& arg) {
    if (polys.empty()) {
        std::cout << "<INVALID COMMAND>\n";
        return;
    }
    if (arg == "AREA") {
        auto it = std::max_element(polys.begin(), polys.end(),
                                   [](const Polygon& a, const Polygon& b) {
                                       return calcArea(a) < calcArea(b);
                                   });
        std::cout << std::fixed << std::setprecision(1) << calcArea(*it)
                  << "\n";
    } else if (arg == "VERTEXES") {
        auto it = std::max_element(polys.begin(), polys.end(),
                                   [](const Polygon& a, const Polygon& b) {
                                       return a.points.size() < b.points.size();
                                   });
        std::cout << it->points.size() << "\n";
    } else {
        std::cout << "<INVALID COMMAND>\n";
    }
}

void doMin(const std::vector<Polygon>& polys, const std::string& arg) {
    if (polys.empty()) {
        std::cout << "<INVALID COMMAND>\n";
        return;
    }
    if (arg == "AREA") {
        auto it = std::min_element(polys.begin(), polys.end(),
                                   [](const Polygon& a, const Polygon& b) {
                                       return calcArea(a) < calcArea(b);
                                   });
        std::cout << std::fixed << std::setprecision(1) << calcArea(*it)
                  << "\n";
    } else if (arg == "VERTEXES") {
        auto it = std::min_element(polys.begin(), polys.end(),
                                   [](const Polygon& a, const Polygon& b) {
                                       return a.points.size() < b.points.size();
                                   });
        std::cout << it->points.size() << "\n";
    } else {
        std::cout << "<INVALID COMMAND>\n";
    }
}

void doCount(const std::vector<Polygon>& polys, const std::string& arg) {
    if (arg == "EVEN") {
        int cnt = std::count_if(polys.begin(), polys.end(),
                                [](const Polygon& p) {
                                    return p.points.size() % 2 == 0;
                                });
        std::cout << cnt << "\n";
    } else if (arg == "ODD") {
        int cnt = std::count_if(polys.begin(), polys.end(),
                                [](const Polygon& p) {
                                    return p.points.size() % 2 != 0;
                                });
        std::cout << cnt << "\n";
    } else {
        int n = 0;
        try {
            n = std::stoi(arg);
        } catch (...) {
            std::cout << "<INVALID COMMAND>\n";
            return;
        }
        if (n < 3) {
            std::cout << "<INVALID COMMAND>\n";
            return;
        }
        int cnt = std::count_if(polys.begin(), polys.end(),
                                [n](const Polygon& p) {
                                    return static_cast<int>(p.points.size()) == n;
                                });
        std::cout << cnt << "\n";
    }
}

void doLessArea(const std::vector<Polygon>& polys, const std::string& arg) {
    std::istringstream ss(arg);
    Polygon target;
    if (!(ss >> target)) {
        std::cout << "<INVALID COMMAND>\n";
        return;
    }
    char c;
    while (ss.get(c)) {
        if (!std::isspace(static_cast<unsigned char>(c))) {
            std::cout << "<INVALID COMMAND>\n";
            return;
        }
    }
    double targetArea = calcArea(target);
    int cnt = std::count_if(polys.begin(), polys.end(),
                            [targetArea](const Polygon& p) {
                                return calcArea(p) < targetArea;
                            });
    std::cout << cnt << "\n";
}

void doSame(const std::vector<Polygon>& polys, const std::string& arg) {
    std::istringstream ss(arg);
    Polygon target;
    if (!(ss >> target)) {
        std::cout << "<INVALID COMMAND>\n";
        return;
    }
    char c;
    while (ss.get(c)) {
        if (!std::isspace(static_cast<unsigned char>(c))) {
            std::cout << "<INVALID COMMAND>\n";
            return;
        }
    }
    int cnt = std::count_if(polys.begin(), polys.end(),
                            [&target](const Polygon& p) {
                                return isSameShape(p, target);
                            });
    std::cout << cnt << "\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file) {
        std::cerr << "Cannot open file: " << argv[1] << "\n";
        return 1;
    }

    std::vector<Polygon> polys;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }
        std::istringstream ss(line);
        Polygon poly;
        if (ss >> poly) {
            char c;
            bool hasGarbage = false;
            while (ss.get(c)) {
                if (!std::isspace(static_cast<unsigned char>(c))) {
                    hasGarbage = true;
                    break;
                }
            }
            if (!hasGarbage) {
                polys.push_back(poly);
            }
        }
    }

    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            continue;
        }
        std::istringstream ss(line);
        std::string cmd;
        ss >> cmd;

        std::string rest;
        std::getline(ss, rest);
        if (!rest.empty() && rest[0] == ' ') {
            rest = rest.substr(1);
        }

        if (cmd == "AREA") {
            std::string arg;
            std::istringstream as(rest);
            if (!(as >> arg)) {
                std::cout << "<INVALID COMMAND>\n";
                continue;
            }
            doArea(polys, arg);
        } else if (cmd == "MAX") {
            std::string arg;
            std::istringstream as(rest);
            if (!(as >> arg)) {
                std::cout << "<INVALID COMMAND>\n";
                continue;
            }
            doMax(polys, arg);
        } else if (cmd == "MIN") {
            std::string arg;
            std::istringstream as(rest);
            if (!(as >> arg)) {
                std::cout << "<INVALID COMMAND>\n";
                continue;
            }
            doMin(polys, arg);
        } else if (cmd == "COUNT") {
            std::string arg;
            std::istringstream as(rest);
            if (!(as >> arg)) {
                std::cout << "<INVALID COMMAND>\n";
                continue;
            }
            doCount(polys, arg);
        } else if (cmd == "LESSAREA") {
            doLessArea(polys, rest);
        } else if (cmd == "SAME") {
            doSame(polys, rest);
        } else {
            std::cout << "<INVALID COMMAND>\n";
        }
    }

    return 0;
}
