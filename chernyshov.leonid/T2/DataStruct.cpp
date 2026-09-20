#include "DataStruct.h"

#include <iomanip>
#include <sstream>
#include <cmath>

std::istream& operator>>(std::istream& in, DelimiterIO&& dest)
{
    char c = '\0';
    in.get(c);

    if (in && c != dest.exp) {
        in.setstate(std::ios::failbit);
    }

    return in;
}

std::istream& operator>>(std::istream& in, UllLitIO&& dest)
{
    unsigned long long value = 0;

    in >> value;

    if (!in) {
        return in;
    }

    char c1 = '\0';
    char c2 = '\0';
    char c3 = '\0';

    in.get(c1);
    in.get(c2);
    in.get(c3);

    if (!in) {
        return in;
    }

    bool isLower = c1 == 'u' && c2 == 'l' && c3 == 'l';
    bool isUpper = c1 == 'U' && c2 == 'L' && c3 == 'L';

    if (!isLower && !isUpper) {
        in.setstate(std::ios::failbit);
        return in;
    }

    dest.ref = value;

    return in;
}

std::istream& operator>>(std::istream& in, ComplexIO&& dest)
{
    char c = '\0';

    in.get(c);

    if (!in || c != '#') {
        in.setstate(std::ios::failbit);
        return in;
    }

    in.get(c);

    if (!in || c != 'c') {
        in.setstate(std::ios::failbit);
        return in;
    }

    in >> DelimiterIO{'('};

    double real = 0.0;
    double imag = 0.0;

    in >> real >> imag;
    in >> DelimiterIO{')'};

    if (in) {
        dest.ref = std::complex<double>(real, imag);
    }

    return in;
}

std::istream& operator>>(std::istream& in, StringIO&& dest)
{
    char c = '\0';

    in.get(c);

    if (!in || c != '"') {
        in.setstate(std::ios::failbit);
        return in;
    }

    std::string value;

    while (in.get(c) && c != '"') {
        value += c;
    }

    if (!in) {
        in.setstate(std::ios::failbit);
        return in;
    }

    dest.ref = value;

    return in;
}

std::istream& operator>>(std::istream& in, DataStruct& dest)
{
    std::istream::sentry sentry(in);

    if (!sentry) {
        return in;
    }

    std::string line;

    while (std::getline(in, line))
    {
        std::istringstream iss(line);

        char c = '\0';

        iss >> std::skipws >> c;

        if (c != '(') {
            continue;
        }

        iss >> std::noskipws;

        iss.get(c);

        if (c != ':') {
            continue;
        }

        DataStruct temp{};

        bool hasKey1 = false;
        bool hasKey2 = false;
        bool hasKey3 = false;

        for (int i = 0; i < 3; ++i)
        {
            std::string key;

            while (iss.get(c) && c != ' ') {
                key += c;
            }

            if (c != ' ') {
                iss.setstate(std::ios::failbit);
                break;
            }

            if (key == "key1") {
                iss >> UllLitIO{temp.key1};
                hasKey1 = true;
            }
            else if (key == "key2") {
                iss >> ComplexIO{temp.key2};
                hasKey2 = true;
            }
            else if (key == "key3") {
                iss >> StringIO{temp.key3};
                hasKey3 = true;
            }
            else {
                iss.setstate(std::ios::failbit);
                break;
            }

            iss >> DelimiterIO{':'};
        }

        iss >> DelimiterIO{')'};

        if (iss && hasKey1 && hasKey2 && hasKey3) {
            dest = temp;
            return in;
        }
    }

    in.setstate(std::ios::failbit);

    return in;
}

std::ostream& operator<<(std::ostream& out, const DataStruct& dest)
{
    out << "(:key1 "
        << dest.key1
        << "ull";

    out << ":key2 #c("
        << std::fixed
        << std::setprecision(1)
        << dest.key2.real()
        << " "
        << dest.key2.imag()
        << ")";

    out << ":key3 \""
        << dest.key3
        << "\":)";

    return out;
}

bool compareDataStruct(const DataStruct& lhs, const DataStruct& rhs)
{
    if (lhs.key1 != rhs.key1) {
        return lhs.key1 < rhs.key1;
    }

    double lhsModule = std::abs(lhs.key2);
    double rhsModule = std::abs(rhs.key2);

    if (lhsModule != rhsModule) {
        return lhsModule < rhsModule;
    }

    return lhs.key3.length() < rhs.key3.length();
}
