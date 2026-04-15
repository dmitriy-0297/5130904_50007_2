#include "DataStruct.h"
#include <iostream>
#include <string>
#include <complex>
#include <algorithm>
#include <iomanip>

namespace loginov
{
  namespace
  {
    struct DelimiterIO { char expected; };
    std::istream & operator>>(std::istream & in, DelimiterIO && dest)
    {
      std::istream::sentry sentry(in);
      if (!sentry) return in;
      char c = '\0';
      in >> c;
      if (in && std::tolower(c) != std::tolower(dest.expected)) in.setstate(std::ios::failbit);
      return in;
    }

    struct LabelIO { std::string expected; };
    std::istream & operator>>(std::istream & in, LabelIO && dest)
    {
      std::istream::sentry sentry(in);
      if (!sentry) return in;
      for (char exp_c : dest.expected)
      {
        in >> DelimiterIO{exp_c};
      }
      return in;
    }

    struct HexUllIO { unsigned long long & value; };
    std::istream & operator>>(std::istream & in, HexUllIO && dest)
    {
      std::istream::sentry sentry(in);
      if (!sentry) return in;
      return in >> std::hex >> dest.value >> std::dec;
    }

    struct ComplexIO { std::complex<double> & value; };
    std::istream & operator>>(std::istream & in, ComplexIO && dest)
    {
      std::istream::sentry sentry(in);
      if (!sentry) return in;
      double re = 0.0, im = 0.0;
      if (!(in >> LabelIO{"#c("} >> re >> im >> DelimiterIO{')'})) return in;
      dest.value = {re, im};
      return in;
    }

    struct StringIO { std::string & value; };
    std::istream & operator>>(std::istream & in, StringIO && dest)
    {
      std::istream::sentry sentry(in);
      if (!sentry) return in;
      return in >> std::quoted(dest.value);
    }
  }

  std::istream & operator>>(std::istream & in, DataStruct & data)
  {
    std::istream::sentry sentry(in);
    if (!sentry) return in;

    DataStruct input;
    if (!(in >> LabelIO{"(:"})) return in;

    for (int i = 0; i < 3; ++i)
    {
      std::string key;
      in >> std::ws;
      std::getline(in, key, ' ');

      if (key == "key1") in >> LabelIO{"0x"} >> HexUllIO{input.key1_};
      else if (key == "key2") in >> ComplexIO{input.key2_};
      else if (key == "key3") in >> StringIO{input.key3_};
      else { in.setstate(std::ios::failbit); return in; }

      in >> DelimiterIO{':'};
    }

    if (!(in >> DelimiterIO{')'})) return in;
    if (in) data = input;
    return in;
  }

  std::ostream & operator<<(std::ostream & out, const DataStruct & data)
  {
    std::ostream::sentry sentry(out);
    if (!sentry) return out;
    std::ios_base::fmtflags f(out.flags());

    out << "(:key1 0x" << std::nouppercase << std::hex << data.key1_;
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
