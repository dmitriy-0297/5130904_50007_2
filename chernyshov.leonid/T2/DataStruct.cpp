#include "DataStruct.h"

#include <iomanip>
#include <sstream>
#include <cctype>

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

    if (in && c != dest.exp)
    {
        in.setstate(std::ios::failbit);
    }

    return in;
}

std::istream& operator>>(std::istream& in, UllLiteralIO&& dest)
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
        if (c == ':')
        {
            in.putback(c);
            break;
        }

        val += c;
    }

    if (val.size() < 4)
    {
        in.setstate(std::ios::failbit);
        return in;
    }

    char c1 = val[val.size() - 3];
    char c2 = val[val.size() - 2];
    char c3 = val[val.size() - 1];

    bool suffix =
        (c1 == 'u' || c1 == 'U') &&
        (c2 == 'l' || c2 == 'L') &&
        (c3 == 'l' || c3 == 'L');

    if (!suffix)
    {
        in.setstate(std::ios::failbit);
        return in;
    }

    std::string number = val.substr(0, val.size() - 3);

    if (number.empty())
    {
        in.setstate(std::ios::failbit);
        return in;
    }

    for (char digit : number)
    {
        if (!std::isdigit(static_cast<unsigned char>(digit)))
        {
            in.setstate(std::ios::failbit);
            return in;
        }
    }

    try
    {
        size_t processed = 0;
        unsigned long long value =
            std::stoull(number, &processed, 10);

        if (processed != number.size())
        {
            in.setstate(std::ios::failbit);
            return in;
        }

        dest.ref = value;
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

    in >> DelimiterIO{'#'};
    in >> DelimiterIO{'c'};
    in >> DelimiterIO{'('};

    double real = 0.0;
    double imag = 0.0;

    in >> real;

    char space = '0';
    in.get(space);

    if (!in || space != ' ')
    {
        in.setstate(std::ios::failbit);
        return in;
    }

    in >> imag;

    in >> DelimiterIO{')'};

    if (in)
    {
        dest.ref = std::complex<double>(real, imag);
    }

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

        DataStruct temp{};

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
                iss >> UllLiteralIO{temp.key1};
                hasKey1 = true;
            }
            else if (key == "key2")
            {
                iss >> ComplexIO{temp.key2};
                hasKey2 = true;
            }
            else if (key == "key3")
            {
                iss >> StringIO{temp.key3};
                hasKey3 = true;
            }
            else
            {
                iss.setstate(std::ios::failbit);
                break;
            }

            if (i < KEYS_COUNT - 1)
            {
                iss >> DelimiterIO{':'};
            }
            else
            {
                iss >> DelimiterIO{':'} >> DelimiterIO{')'};
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

    out << "(:key1 "
        << src.key1
        << "ull";

    out << ":key2 #c("
        << std::fixed
        << std::setprecision(1)
        << src.key2.real()
        << " "
        << src.key2.imag()
        << ")";

    out << ":key3 \""
        << src.key3
        << "\":)";

    return out;
}

bool compareDataStruct(const DataStruct& a, const DataStruct& b)
{
    if (a.key1 != b.key1)
    {
        return a.key1 < b.key1;
    }

    double moduleA = std::abs(a.key2);
    double moduleB = std::abs(b.key2);

    if (moduleA != moduleB)
    {
        return moduleA < moduleB;
    }

    return a.key3.length() < b.key3.length();
}
