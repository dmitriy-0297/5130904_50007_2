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
    std::istream::sentry sentry(in, true);
    if (!sentry)
    {
        return in;
    }
    char c = '0';
    in.get(c);
    if (in && (c != dest.exp))
    {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::istream& operator>>(std::istream& in, SignedLongLongIO&& dest)
{
    std::istream::sentry sentry(in, true);
    if (!sentry)
    {
        return in;
    }
    std::string val;
    in >> val;
    if (val.length() < 3)
    {
        in.setstate(std::ios::failbit);
        return in;
    }
    std::string suffix = val.substr(val.length() - 2);
    if (suffix != "ll" && suffix != "LL")
    {
        in.setstate(std::ios::failbit);
        return in;
    }
    try
    {
        dest.ref = std::stoll(val.substr(0, val.length() - 2));
    }
    catch (...)
    {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::istream& operator>>(std::istream& in, ComplexIO&& dest)
{
    std::istream::sentry sentry(in, true);
    if (!sentry)
    {
        return in;
    }
    char c = '0';
    in.get(c);
    if (c != '#')
    {
        in.setstate(std::ios::failbit);
        return in;
    }
    in.get(c);
    if (c != 'c')
    {
        in.setstate(std::ios::failbit);
        return in;
    }
    in.get(c);
    if (c != '(')
    {
        in.setstate(std::ios::failbit);
        return in;
    }

    double realPart = 0.0;
    double imagPart = 0.0;
    in >> realPart >> imagPart;
    if (!in)
    {
        in.setstate(std::ios::failbit);
        return in;
    }

    in.get(c);
    if (c != ')')
    {
        in.setstate(std::ios::failbit);
        return in;
    }

    dest.ref = std::complex<double>(realPart, imagPart);
    return in;
}

std::istream& operator>>(std::istream& in, StringIO&& dest)
{
    std::istream::sentry sentry(in, true);
    if (!sentry)
    {
        return in;
    }
    char c = '0';
    in.get(c);
    if (c != '"')
    {
        in.setstate(std::ios::failbit);
        return in;
    }
    std::string val;
    while (in.get(c) && c != '"')
    {
        val += c;
    }
    if (c == '"')
    {
        dest.ref = val;
    }
    else
    {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::istream& operator>>(std::istream& in, DataStruct& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }

    std::string line;
    while (std::getline(in, line))
    {
        std::istringstream iss(line);
        char startChar = '0';
        iss >> std::skipws >> startChar;
        if (startChar != '(')
        {
            continue;
        }

        iss >> std::noskipws;
        char nextChar = '0';
        iss.get(nextChar);
        if (nextChar != ':')
        {
            continue;
        }

        DataStruct temp{};
        bool hasKey1 = false;
        bool hasKey2 = false;
        bool hasKey3 = false;
        const size_t FIELDS_COUNT = 3;

        for (size_t i = 0; i < FIELDS_COUNT; ++i)
        {
            std::string key;
            char c = '0';
            while (iss.get(c) && c != ' ')
            {
                key += c;
            }
            if (c != ' ')
            {
                iss.setstate(std::ios::failbit);
                break;
            }

            if (key == ":key1")
            {
                iss >> SignedLongLongIO{ temp.key1_ };
                hasKey1 = true;
            }
            else if (key == ":key2")
            {
                iss >> ComplexIO{ temp.key2_ };
                hasKey2 = true;
            }
            else if (key == ":key3")
            {
                iss >> StringIO{ temp.key3_ };
                hasKey3 = true;
            }
            else
            {
                iss.setstate(std::ios::failbit);
                break;
            }

            if (i < FIELDS_COUNT - 1)
            {
                iss >> DelimiterIO{ ':' };
            }
            else
            {
                iss >> DelimiterIO{ ':' } >> DelimiterIO{ ')' };
            }
        }

        if (iss && hasKey1 && hasKey2 && hasKey3)
        {
            dest = std::move(temp);
            return in;
        }
    }
    in.setstate(std::ios::failbit);
    return in;
}

std::ostream& operator<<(std::ostream& out, const DataStruct& src)
{
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
        return out;
    }
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
