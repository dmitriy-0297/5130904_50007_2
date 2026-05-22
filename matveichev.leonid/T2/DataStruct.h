#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <iostream>
#include <string>

struct DataStruct
{
  double key1_;
  char key2_;
  std::string key3_;
};

struct DelimiterIO
{
  char exp_;
};

struct DoubleScientificIO
{
  double& ref_;
};

struct CharLiteralIO
{
  char& ref_;
};

struct StringIO
{
  std::string& ref_;
};

class IoFmtGuard
{
public:
  IoFmtGuard(std::basic_ios<char>& s);
  ~IoFmtGuard();
private:
  std::basic_ios<char>& s_;
  std::streamsize width_;
  char fill_;
  std::streamsize precision_;
  std::basic_ios<char>::fmtflags fmt_;
};

std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
std::istream& operator>>(std::istream& in, DoubleScientificIO&& dest);
std::istream& operator>>(std::istream& in, CharLiteralIO&& dest);
std::istream& operator>>(std::istream& in, StringIO&& dest);
std::istream& operator>>(std::istream& in, DataStruct& dest);
std::ostream& operator<<(std::ostream& out, const DataStruct& src);
bool compareDataStruct(const DataStruct& a, const DataStruct& b);

#endif
