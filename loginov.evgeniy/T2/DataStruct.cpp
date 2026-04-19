#include "DataStruct.h"
#include <iostream>
#include <string>
#include <complex>
#include <algorithm>
#include <iomanip>

namespace loginov
{
  struct DelimiterIO { char expected; };

  std::istream & operator>>(std::istream & in, DelimiterIO && dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) return in;
    char c = '\0';
    in >> c;
    if (in && std::tolower(c) != std::tolower(dest.expected))
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::istream & operator>>(std::istream & in, DataStruct & data)
  {
    std::istream::sentry sentry(in);
    if (!sentry) return in;

    DataStruct temp{0, {0.0, 0.0}, ""};
    in >> DelimiterIO{'('} >> DelimiterIO{':'};

    for (int i = 0; i < 3; ++i)
    {
      char k, e, y;
      in >> k >> e >> y;
      if (k != 'k' || e != 'e' || y != 'y')
      {
        in.setstate(std::ios::failbit);
        return in;
      }

      int keyNum = 0;
      in >> keyNum;

      if (keyNum == 1)
      {
        char zero, x;
        in >> zero >> x;
        if (zero != '0' || std::tolower(x) != 'x')
        {
          in.setstate(std::ios::failbit);
          return in;
        }
        in >> std::hex >> temp.key1_ >> std::dec;
      }
      else if (keyNum == 2)
      {
        char hash, c, openParen, closeParen;
        double re = 0.0, im = 0.0;
        in >> hash >> c >> openParen >> re >> im >> closeParen;
        if (hash != '#' || std::tolower(c) != 'c' || openParen != '(' || closeParen != ')')
        {
          in.setstate(std::ios::failbit);
          return in;
        }
        temp.key2_ = {re, im};
      }
      else if (keyNum == 3)
      {
        in >> std::quoted(temp.key3_);
      }
      else
      {
        in.setstate(std::ios::failbit);
        return in;
      }
      in >> DelimiterIO{':'};
    }

    in >> DelimiterIO{')'};
    if (in) data = temp;
    return in;
  }

  std::ostream & operator<<(std::ostream & out, const DataStruct & data)
  {
    std::ostream::sentry sentry(out);
    if (!sentry) return out;
    std::ios_base::fmtflags f(out.flags());

    out << "(:key1 0x" << std::uppercase << std::hex << data.key1_ << std::nouppercase << std::dec;
    out << ":key2 #c(" << std::fixed << std::setprecision(1) << data.key2_.real() << " " << data.key2_.imag() << ")";
    out << ":key3 " << std::quoted(data.key3_) << ":)";

    out.flags(f);
    return out;
  }

  bool DataStructComparator::operator()(const DataStruct & lhs, const DataStruct & rhs) const
  {
    if (lhs.key1_ != rhs.key1_) return lhs.key1_ < rhs.key1_;
    if (std::abs(lhs.key2_) != std::abs(rhs.key2_)) return std::abs(lhs.key2_) < std::abs(rhs.key2_);
    return lhs.key3_.length() < rhs.key3_.length();
  }
}
