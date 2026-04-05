#include "DataStruct.h"
#include <iomanip>
#include <sstream>
#include <cctype>
#include <cmath>

iofmtguard::iofmtguard(std::basic_ios<char>& s) :
    s_(s),
    width_(s.width()),
    fill_(s.fill()),
    precision_(s.precision()),
    fmt_(s.flags())
{
}

iofmtguard::~iofmtguard()
{
    s_.width(width_);
    s_.fill(fill_);
    s_.precision(precision_);
    s_.flags(fmt_);
}

std::istream& operator>>(std::istream& in, DelimiterIO&& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }
    char c = 0;
    in >> c;
    if (in && c != dest.exp) {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::istream& operator>>(std::istream& in, SignedLongLongIO&& dest)
{
    std::istream::sentry sentry(in, true);
    if (!sentry) {
        return in;
    }
    long long value = 0;
    if (!(in >> value)) {
        return in;
    }
    char c1 = 0, c2 = 0;
    in.get(c1);
    in.get(c2);
    if (in && std::tolower(static_cast<unsigned char>(c1)) == 'l' &&
        std::tolower(static_cast<unsigned char>(c2)) == 'l') {
        dest.ref = value;
    }
    else {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::istream& operator>>(std::istream& in, ComplexIO&& dest)
{
    std::istream::sentry sentry(in, true);
    if (!sentry) {
        return in;
    }
    using sep = DelimiterIO;
    in >> sep{ '#' } >> sep{ 'c' } >> sep{ '(' };
    if (!in) {
        return in;
    }
    double realPart = 0.0, imagPart = 0.0;
    in >> realPart >> imagPart;
    if (!in) {
        return in;
    }
    in >> sep{ ')' };
    if (in) {
        dest.ref = std::complex<double>(realPart, imagPart);
    }
    return in;
}

std::istream& operator>>(std::istream& in, StringIO&& dest)
{
    std::istream::sentry sentry(in, true);
    if (!sentry) {
        return in;
    }
    in >> DelimiterIO{ '"' };
    if (!in) {
        return in;
    }
    std::string val;
    std::getline(in, val, '"');
    if (in) {
        dest.ref = val;
    }
    else {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::istream& operator>>(std::istream& in, DataStruct& dest)
{
    std::istream::sentry sentry(in, true);
    if (!sentry) {
        return in;
    }

    std::string line;
    while (std::getline(in, line)) {
        std::istringstream iss(line);
        iss >> std::skipws;

        char c = 0;
        if (!(iss >> c) || c != '(') {
            continue;
        }
        if (!(iss >> c) || c != ':') {
            continue;
        }

        DataStruct temp{};
        bool hasKey1 = false, hasKey2 = false, hasKey3 = false;
        bool valid = true;

        for (int i = 0; i < 3; ++i) {
            std::string key;
            char kc = 0;
            while (iss.get(kc) && std::isalnum(static_cast<unsigned char>(kc))) {
                key += kc;
            }
            if (kc != ' ') {
                valid = false;
                break;
            }

            if (key == "key1") {
                if (!(iss >> SignedLongLongIO{ temp.key1_ })) {
                    valid = false;
                    break;
                }
                hasKey1 = true;
            }
            else if (key == "key2") {
                if (!(iss >> ComplexIO{ temp.key2_ })) {
                    valid = false;
                    break;
                }
                hasKey2 = true;
            }
            else if (key == "key3") {
                if (!(iss >> StringIO{ temp.key3_ })) {
                    valid = false;
                    break;
                }
                hasKey3 = true;
            }
            else {
                valid = false;
                break;
            }

            if (!(iss >> DelimiterIO{ ':' })) {
                valid = false;
                break;
            }
        }

        if (!valid) {
            if (iss.fail()) iss.clear();
            continue;
        }

        if (!(iss >> DelimiterIO{ ')' })) {
            if (iss.fail()) iss.clear();
            continue;
        }

        if (hasKey1 && hasKey2 && hasKey3) {
            dest = std::move(temp);
            return in;
        }
        if (iss.fail()) iss.clear();
    }

    in.setstate(std::ios::failbit);
    return in;
}

std::ostream& operator<<(std::ostream& out, const DataStruct& src)
{
    std::ostream::sentry sentry(out);
    if (!sentry) {
        return out;
    }
    iofmtguard fmtguard(out);

    out << "(:key1 " << src.key1_ << "ll:";
    out << "key2 #c(" << std::fixed << std::setprecision(1);
    out << std::real(src.key2_) << " " << std::imag(src.key2_) << "):";
    out << "key3 \"" << src.key3_ << "\":)";

    return out;
}

bool compareDataStruct(const DataStruct& a, const DataStruct& b)
{
    if (a.key1_ != b.key1_) {
        return a.key1_ < b.key1_;
    }
    double modA = std::abs(a.key2_);
    double modB = std::abs(b.key2_);
    if (modA != modB) {
        return modA < modB;
    }
    return a.key3_.length() < b.key3_.length();
}
