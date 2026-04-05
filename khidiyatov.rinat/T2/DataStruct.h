#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <iostream>
#include <string>
#include <complex>

struct DataStruct
{
    long long key1_;
    std::complex<double> key2_;
    std::string key3_;
};

struct DelimiterIO
{
    char exp;
};

struct SignedLongLongIO
{
    long long& ref;
};

struct ComplexIO
{
    std::complex<double>& ref;
};

struct StringIO
{
    std::string& ref;
};

class iofmtguard
{
public:
    explicit iofmtguard(std::basic_ios<char>& s);
    ~iofmtguard();
private:
    std::basic_ios<char>& s_;
    std::streamsize width_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios<char>::fmtflags fmt_;
};

std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
std::istream& operator>>(std::istream& in, SignedLongLongIO&& dest);
std::istream& operator>>(std::istream& in, ComplexIO&& dest);
std::istream& operator>>(std::istream& in, StringIO&& dest);
std::istream& operator>>(std::istream& in, DataStruct& dest);
std::ostream& operator<<(std::ostream& out, const DataStruct& src);
bool compareDataStruct(const DataStruct& a, const DataStruct& b);

#endif
