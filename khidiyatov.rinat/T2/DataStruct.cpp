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
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }
    std::string token;
    in >> token;
    if (token.length() < 3) {
        in.setstate(std::ios::failbit);
        return in;
    }
    std::string suffix = token.substr(token.length() - 2);
    if (suffix != "ll" && suffix != "LL") {
        in.setstate(std::ios::failbit);
        return in;
    }
    try {
        std::string numPart = token.substr(0, token.length() - 2);
        dest.ref = std::stoll(numPart);
    }
    catch (...) {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::istream& operator>>(std::istream& in, ComplexIO&& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }
    char c = 0;
    in >> c;
    if (c != '#') {
        in.setstate(std::ios::failbit);
        return in;
    }
    in >> c;
    if (std::tolower(static_cast<unsigned char>(c)) != 'c') {
        in.setstate(std::ios::failbit);
        return in;
    }
    in >> c;
    if (c != '(') {
        in.setstate(std::ios::failbit);
        return in;
    }
    double realPart = 0.0;
    double imagPart = 0.0;
    if (!(in >> realPart >> imagPart)) {
        return in;
    }
    in >> c;
    if (c != ')') {
        in.setstate(std::ios::failbit);
        return in;
    }
    dest.ref = std::complex<double>(realPart, imagPart);
    return in;
}

std::istream& operator>>(std::istream& in, StringIO&& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }
    char c = 0;
    in >> c;
    if (c != '"') {
        in.setstate(std::ios::failbit);
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
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }
    std::string line;
    while (std::getline(in, line)) {
        std::istringstream iss(line);
        char c = 0;
        iss >> std::skipws >> c;
        if (c != '(') {
            continue;
        }
        iss >> c;
        if (c != ':') {
            continue;
        }
        DataStruct temp{};
        bool hasKey1 = false;
        bool hasKey2 = false;
        bool hasKey3 = false;
        bool valid = true;
        for (int i = 0; i < 3; ++i) {
            std::string key;
            char kc = 0;
            while (iss.get(kc) && kc != ' ' && kc != ':') {
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
        if (valid && (iss >> DelimiterIO{ ')' }) && hasKey1 && hasKey2 && hasKey3) {
            dest = std::move(temp);
            return in;
        }
        if (iss.fail() && !iss.eof()) {
            iss.clear();
        }
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
    out << "(:key1 " << src.key1_ << "ll ";
    out << ":key2#c(" << std::fixed << std::setprecision(1);
    out << std::real(src.key2_) << " " << std::imag(src.key2_) << ") ";
    out << ":key3\"" << src.key3_ << "\":)";
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
