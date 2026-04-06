#include "DataStruct.h"
#include <iomanip>
#include <sstream>
#include <cctype>
#include <algorithm>

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

std::istream& operator>>(std::istream& in, UllLitIO&& dest)
{
    std::istream::sentry sentry(in, true);
    if (!sentry)
    {
        return in;
    }
    unsigned long long v = 0;
    in >> v;
    if (in)
    {
        char suf[3] = {0, 0, 0};
        in.get(suf, 3);
        if (in && (std::tolower(suf[0]) == 'u' && 
                   std::tolower(suf[1]) == 'l' && 
                   std::tolower(suf[2]) == 'l'))
        {
            dest.ref = v;
        }
        else
        {
            in.setstate(std::ios::failbit);
        }
    }
    return in;
}

std::istream& operator>>(std::istream& in, UllBinIO&& dest)
{
    std::istream::sentry sentry(in, true);
    if (!sentry)
    {
        return in;
    }
    char p[2] = {0, 0};
    in.get(p, 3);
    if (in && (p[0] == '0') && (std::tolower(p[1]) == 'b'))
    {
        unsigned long long v = 0;
        in >> std::noskipws;
        char c = 0;
        while (in.get(c) && (c == '0' || c == '1'))
        {
            v = (v << 1) | (c - '0');
        }
        in >> std::skipws;
        if (in || in.eof())
        {
            dest.ref = v;
            in.clear();
        }
        else
        {
            in.setstate(std::ios::failbit);
        }
    }
    else
    {
        in.setstate(std::ios::failbit);
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
    
    // Пропускаем пробелы перед кавычкой
    in >> std::skipws;
    
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
    
    DataStruct temp;
    bool hasKey1 = false;
    bool hasKey2 = false;
    bool hasKey3 = false;
    
    using sep = DelimiterIO;
    using ull = UllLitIO;
    using bin = UllBinIO;
    using str = StringIO;
    
    if (!(in >> sep{ '(' } >> sep{ ':' }))
    {
        in.setstate(std::ios::failbit);
        return in;
    }
    
    char c = 0;
    std::string key;
    
    while (in && !(hasKey1 && hasKey2 && hasKey3))
    {
        in >> std::skipws >> c;
        
        if (c == ')')
        {
            break;
        }
        in.putback(c);
        
        key.clear();
        while (in.get(c) && c != ' ' && c != '"')
        {
            key += c;
        }
        
        if (key == "key1" && !hasKey1)
        {
            if (c == ' ')
            {
                if (in >> ull{ temp.key1 })
                {
                    hasKey1 = true;
                }
                else
                {
                    break;
                }
            }
            else
            {
                in.setstate(std::ios::failbit);
                break;
            }
        }
        else if (key == "key2" && !hasKey2)
        {
            if (c == ' ')
            {
                if (in >> bin{ temp.key2 })
                {
                    hasKey2 = true;
                }
                else
                {
                    break;
                }
            }
            else
            {
                in.setstate(std::ios::failbit);
                break;
            }
        }
        else if (key == "key3" && !hasKey3)
        {
            if (c == '"' || c == ' ')
            {
                if (c == ' ')
                {
                    if (in >> str{ temp.key3 })
                    {
                        hasKey3 = true;
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    in.putback(c);
                    if (in >> str{ temp.key3 })
                    {
                        hasKey3 = true;
                    }
                    else
                    {
                        break;
                    }
                }
            }
            else
            {
                in.setstate(std::ios::failbit);
                break;
            }
        }
        else
        {
            in.setstate(std::ios::failbit);
            break;
        }
        
        if (!(in >> std::skipws >> sep{ ':' }))
        {
            break;
        }
    }
    
    if (in && hasKey1 && hasKey2 && hasKey3)
    {
        in >> sep{ ')' };
    }
    
    if (in && hasKey1 && hasKey2 && hasKey3)
    {
        dest = std::move(temp);
    }
    else
    {
        in.setstate(std::ios::failbit);
    }
    
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
    out << "(:key1 " << src.key1 << "ull:key2 0b";
    if (src.key2 == 0)
    {
        out << "0";
    }
    else
    {
        std::string bin;
        unsigned long long t = src.key2;
        while (t > 0)
        {
            bin = ((t & 1) ? "1" : "0") + bin;
            t >>= 1;
        }
        out << bin;
    }
    out << ":key3 \"" << src.key3 << "\":)";
    return out;
}

bool compareDataStruct(const DataStruct& a, const DataStruct& b)
{
    if (a.key1 != b.key1)
    {
        return a.key1 < b.key1;
    }
    if (a.key2 != b.key2)
    {
        return a.key2 < b.key2;
    }
    return a.key3.length() < b.key3.length();
}