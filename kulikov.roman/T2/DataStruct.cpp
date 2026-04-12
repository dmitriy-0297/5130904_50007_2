#include "DataStruct.h"

#include <iostream>
#include <string>

namespace kulikov
{
  namespace
  {
    struct DelimiterIO
    {
      char expected;
    };

    struct UllLitIO
    {
      unsigned long long & value;
    };

    struct UllBinIO
    {
      unsigned long long & value;
      std::string & bin;
    };

    struct StringIO
    {
      std::string & value;
    };

    std::istream & operator>>(std::istream & in, DelimiterIO && dest)
    {
      char c = '\0';
      in >> c;
      if (in && c != dest.expected)
      {
        in.setstate(std::ios::failbit);
      }
      return in;
    }

    std::istream & operator>>(std::istream & in, UllLitIO && dest)
    {
      unsigned long long val = 0;
      in >> val;
      if (!in)
      {
        return in;
      }
      char c1 = '\0';
      char c2 = '\0';
      char c3 = '\0';
      in.get(c1);
      in.get(c2);
      in.get(c3);
      if (!in)
      {
        return in;
      }
      bool isLower = (c1 == 'u' && c2 == 'l' && c3 == 'l');
      bool isUpper = (c1 == 'U' && c2 == 'L' && c3 == 'L');
      if (!isLower && !isUpper)
      {
        in.setstate(std::ios::failbit);
        return in;
      }
      dest.value = val;
      return in;
    }

    std::istream & operator>>(std::istream & in, UllBinIO && dest)
    {
      char c0 = '\0';
      char cB = '\0';
      in >> c0 >> cB;
      if (!in || c0 != '0' || (cB != 'b' && cB != 'B'))
      {
        in.setstate(std::ios::failbit);
        return in;
      }
      unsigned long long val = 0;
      std::string digits;
      char ch = '\0';
      while (in.get(ch) && (ch == '0' || ch == '1'))
      {
        digits += ch;
      }
      if (in)
      {
        in.putback(ch);
      }
      if (digits.empty())
      {
        in.setstate(std::ios::failbit);
        return in;
      }
      for (char bit : digits)
      {
        val = (val << 1) | (bit - '0');
      }
      dest.value = val;
      dest.bin = digits;
      return in;
    }

    std::istream & operator>>(std::istream & in, StringIO && dest)
    {
      char c = '\0';
      in >> c;
      if (!in || c != '"')
      {
        in.setstate(std::ios::failbit);
        return in;
      }
      std::string result;
      while (in.get(c) && c != '"')
      {
        result += c;
      }
      if (!in)
      {
        in.setstate(std::ios::failbit);
        return in;
      }
      dest.value = result;
      return in;
    }
  }

  std::istream & operator>>(std::istream & in, DataStruct & data)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    DataStruct tmp{};
    bool hasKey1 = false;
    bool hasKey2 = false;
    bool hasKey3 = false;
    in >> DelimiterIO{'('};
    if (!in)
    {
      return in;
    }
    while (true)
    {
      char colon = '\0';
      in >> colon;
      if (!in)
      {
        break;
      }
      if (colon == ':')
      {
        char peek = in.peek();
        if (peek == ')')
        {
          in.get();
          break;
        }
        std::string key;
        if (!(in >> key))
        {
          break;
        }
        if (key == "key1")
        {
          if (!(in >> UllLitIO{tmp.key1}))
          {
            break;
          }
          hasKey1 = true;
        }
        else if (key == "key2")
        {
          if (!(in >> UllBinIO{tmp.key2, tmp.key2Bin}))
          {
            break;
          }
          hasKey2 = true;
        }
        else if (key == "key3")
        {
          if (!(in >> StringIO{tmp.key3}))
          {
            break;
          }
          hasKey3 = true;
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
    }
    if (in && hasKey1 && hasKey2 && hasKey3)
    {
      data = tmp;
    }
    else
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::ostream & operator<<(std::ostream & out, const DataStruct & data)
  {
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
      return out;
    }
    out << "(:key1 " << data.key1 << "ull";
    out << ":key2 0b" << data.key2Bin;
    out << ":key3 \"" << data.key3 << "\":)";
    return out;
  }

  bool DataStructComparator::operator()(const DataStruct & lhs, const DataStruct & rhs) const
  {
    if (lhs.key1 != rhs.key1)
    {
      return lhs.key1 < rhs.key1;
    }
    if (lhs.key2 != rhs.key2)
    {
      return lhs.key2 < rhs.key2;
    }
    return lhs.key3.size() < rhs.key3.size();
  }
}
