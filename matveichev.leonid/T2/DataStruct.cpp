#include "DataStruct.h"
#include <iomanip>
#include <sstream>
#include <cctype>
#include <cmath>

IoFmtGuard::IoFmtGuard(std::basic_ios<char>& s) :
  s_(s),
  width_(s.width()),
  fill_(s.fill()),
  precision_(s.precision()),
  fmt_(s.flags())
{
}

IoFmtGuard::~IoFmtGuard()
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
  if (in && (c != dest.exp_))
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& operator>>(std::istream& in, DoubleScientificIO&& dest)
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
    if (std::isspace(static_cast<unsigned char>(c)) || c == ':')
    {
      in.putback(c);
      break;
    }
    val += c;
  }

  if (val.empty())
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  std::size_t ePos = std::string::npos;
  for (std::size_t i = 0; i < val.size(); ++i)
  {
    if (val[i] == 'e' || val[i] == 'E')
    {
      ePos = i;
      break;
    }
  }

  if (ePos == std::string::npos)
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  std::string mantissa = val.substr(0, ePos);
  std::size_t startIdx = 0;
  if (!mantissa.empty() && mantissa[0] == '-')
  {
    startIdx = 1;
  }

  std::size_t dotPos = mantissa.find('.');
  if (dotPos == std::string::npos || dotPos == startIdx ||
      dotPos == mantissa.size() - 1)
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  if (!std::isdigit(static_cast<unsigned char>(mantissa[dotPos - 1])) ||
      !std::isdigit(static_cast<unsigned char>(mantissa[dotPos + 1])))
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  try
  {
    std::size_t processed = 0;
    dest.ref_ = std::stod(val, &processed);
    if (processed != val.size())
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

std::istream& operator>>(std::istream& in, CharLiteralIO&& dest)
{
  std::istream::sentry sentry(in, true);
  if (!sentry)
  {
    return in;
  }

  char open = '0';
  in.get(open);
  if (!in || open != '\'')
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  char value = '0';
  char close = '0';
  in.get(value);
  in.get(close);

  if (!in || close != '\'')
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  dest.ref_ = value;
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
    dest.ref_ = val;
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
        iss >> DoubleScientificIO{temp.key1_};
        hasKey1 = true;
      }
      else if (key == "key2")
      {
        iss >> CharLiteralIO{temp.key2_};
        hasKey2 = true;
      }
      else if (key == "key3")
      {
        iss >> StringIO{temp.key3_};
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

  IoFmtGuard fmtguard(out);

  double value = src.key1_;
  int exponent = 0;
  if (value != 0.0)
  {
    exponent = static_cast<int>(std::floor(std::log10(std::fabs(value))));
    value /= std::pow(10.0, exponent);
  }

  out << "(:key1 ";
  out << std::fixed << std::setprecision(1) << value;
  if (exponent >= 0)
  {
    out << "e+" << exponent;
  }
  else
  {
    out << "e-" << -exponent;
  }
  out << ":key2 '" << src.key2_ << "'";
  out << ":key3 \"" << src.key3_ << "\"";
  out << ":)";

  return out;
}

bool compareDataStruct(const DataStruct& a, const DataStruct& b)
{
  if (a.key1_ != b.key1_)
  {
    return a.key1_ < b.key1_;
  }
  if (a.key2_ != b.key2_)
  {
    return a.key2_ < b.key2_;
  }
  return a.key3_.length() < b.key3_.length();
}
