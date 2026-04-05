#include "DataStruct.h"
#include <iomanip>
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
    if (!sentry) return in;
    char c = '0';
    in >> c;
    if (in && c != dest.exp)
    {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::istream& operator>>(std::istream& in, SignedLongLongIO&& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry) return in;

    long long temp;
    if (!(in >> temp)) return in;

    char c1 = 0, c2 = 0;
    in.get(c1);
    in.get(c2);

    if (in && std::tolower(static_cast<unsigned char>(c1)) == 'l' &&
        std::tolower(static_cast<unsigned char>(c2)) == 'l')
    {
        dest.ref = temp;
    }
    else
    {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::istream& operator>>(std::istream& in, ComplexIO&& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry) return in;

    char c = 0;
    in >> c;
    if (c != '#') { in.setstate(std::ios::failbit); return in; }

    in >> c;
    if (std::tolower(static_cast<unsigned char>(c)) != 'c') { in.setstate(std::ios::failbit); return in; }

    in >> c;
    if (c != '(') { in.setstate(std::ios::failbit); return in; }

    double realPart = 0.0, imagPart = 0.0;
    if (!(in >> realPart >> imagPart)) return in;

    in >> c;
    if (c != ')') { in.setstate(std::ios::failbit); return in; }

    dest.ref = std::complex<double>(realPart, imagPart);
    return in;
}

std::istream& operator>>(std::istream& in, StringIO&& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry) return in;

    char c = 0;
    in >> c;
    if (c != '"')
    {
        in.setstate(std::ios::failbit);
        return in;
    }

    std::string val;
    std::getline(in, val, '"');
    if (in)
    {
        dest.ref = val;
    }
    return in;
}

std::istream& operator>>(std::istream& in, DataStruct& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry) return in;

    while (true)
    {
        char c = 0;

        while (in >> c && c != '(') {}
        if (!in) return in;

        in >> c;
        if (!in || c != ':')
        {
            if (in.fail()) in.clear();
            continue;
        }

        DataStruct temp{};
        bool hasKey1 = false, hasKey2 = false, hasKey3 = false;
        bool valid = true;

        for (int i = 0; i < 3; ++i)
        {
            in >> std::ws;
            std::string key;
            char kc = 0;

            while (in.get(kc) && (std::isalpha(static_cast<unsigned char>(kc)) ||
                std::isdigit(static_cast<unsigned char>(kc))))
            {
                key += kc;
            }
            if (in) in.unget();

            if (key == "key1")
            {
                if (!(in >> SignedLongLongIO{ temp.key1_ })) valid = false;
                else hasKey1 = true;
            }
            else if (key == "key2")
            {
                if (!(in >> ComplexIO{ temp.key2_ })) valid = false;
                else hasKey2 = true;
            }
            else if (key == "key3")
            {
                if (!(in >> StringIO{ temp.key3_ })) valid = false;
                else hasKey3 = true;
            }
            else
            {
                valid = false;
            }

            if (!valid) break;

            if (!(in >> DelimiterIO{ ':' }))
            {
                valid = false;
                break;
            }
        }

        if (!valid)
        {
            if (in.fail()) in.clear();
            continue;
        }

        if (!(in >> DelimiterIO{ ')' }))
        {
            if (in.fail()) in.clear();
            continue;
        }

        if (hasKey1 && hasKey2 && hasKey3)
        {
            dest = temp;
            return in;
        }
        else
        {
            if (in.fail()) in.clear();
        }
    }
}

std::ostream& operator<<(std::ostream& out, const DataStruct& src)
{
    std::ostream::sentry sentry(out);
    if (!sentry) return out;

    iofmtguard fmtguard(out);
    out << "(:key1 " << src.key1_ << "ll";
    out << ":key2#c(" << std::fixed << std::setprecision(1) << std::real(src.key2_) << " " << std::imag(src.key2_) << ")";
    out << ":key3\"" << src.key3_ << "\":)";
    return out;
}

bool compareDataStruct(const DataStruct& a, const DataStruct& b)
{
    if (a.key1_ != b.key1_)
    {
        return a.key1_ < b.key1_;
    }

    double modA = std::abs(a.key2_);
    double modB = std::abs(b.key2_);

    if (modA != modB)
    {
        return modA < modB;
    }
    return a.key3_.length() < b.key3_.length();
}
