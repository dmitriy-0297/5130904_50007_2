#include "DataStruct.h"
#include <iomanip>
#include <sstream>
#include <cctype>

iofmtguard::iofmtguard(std::basic_ios< char >& s) :
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

std::istream& operator>>(std::istream& in, DoubleLiteralIO&& dest)
{
    std::istream::sentry sentry(in, true);
    if (!sentry)
    {
        return in;
    }

    std::string val = "";
    char c = '0';
    while (in.get(c))
    {
        val += c;
        if (c == 'd' || c == 'D')
        {
            break;
        }
    }

    if (val.empty() || (val.back() != 'd' && val.back() != 'D'))
    {
        in.setstate(std::ios::failbit);
        return in;
    }

    size_t dotPos = val.find('.');
    if (dotPos == std::string::npos || dotPos == 0 || dotPos == val.length() - 2)
    {
        in.setstate(std::ios::failbit);
        return in;
    }

    if (!std::isdigit(static_cast<unsigned char>(val[dotPos - 1])) ||
        !std::isdigit(static_cast<unsigned char>(val[dotPos + 1])))
    {
        in.setstate(std::ios::failbit);
        return in;
    }

    try
    {
        size_t processed = 0;
        dest.ref = std::stod(val.substr(0, val.length() - 1), &processed);
        if (processed != val.length() - 1)
        {
            in.setstate(std::ios::failbit);
        }
    }
    catch (...)
    {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::istream& operator>>(std::istream& in, RationalIO&& dest)
{
    std::istream::sentry sentry(in, true);
    if (!sentry)
    {
        return in;
    }
    using sep = DelimiterIO;
    in >> sep{ '(' } >> sep{ ':' } >> sep{ 'N' };
    in >> std::skipws >> dest.ref.first >> std::noskipws;
    in >> sep{ ':' } >> sep{ 'D' };
    in >> std::skipws >> dest.ref.second >> std::noskipws;
    in >> sep{ ':' } >> sep{ ')' };
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
    std::string val = "";
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

    std::string line = "";
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

        DataStruct temp;
        bool hasKey1 = false;
        bool hasKey2 = false;
        bool hasKey3 = false;
        const int KEYS_COUNT = 3;

        for (int i = 0; i < KEYS_COUNT; ++i)
        {
            std::string key = "";
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

            if (key == "key1")
            {
                iss >> DoubleLiteralIO{ temp.key1 };
                hasKey1 = true;
            }
            else if (key == "key2")
            {
                iss >> RationalIO{ temp.key2 };
                hasKey2 = true;
            }
            else if (key == "key3")
            {
                iss >> StringIO{ temp.key3 };
                hasKey3 = true;
            }
            else
            {
                iss.setstate(std::ios::failbit);
                break;
            }

            if (i < KEYS_COUNT - 1)
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

    out << "(:key1 " << std::fixed << std::setprecision(1) << src.key1 << "d";
    out << ":key2 (:N " << src.key2.first << ":D " << src.key2.second << ":)";
    out << ":key3 \"" << src.key3 << "\":)";

    return out;
}

bool compareDataStruct(const DataStruct& a, const DataStruct& b)
{
    if (a.key1 != b.key1)
    {
        return a.key1 < b.key1;
    }
    long double valA = static_cast<long double>(a.key2.first) / a.key2.second;
    long double valB = static_cast<long double>(b.key2.first) / b.key2.second;
    if (valA != valB)
    {
        return valA < valB;
    }
    return a.key3.length() < b.key3.length();
}
