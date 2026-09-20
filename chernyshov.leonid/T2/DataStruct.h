#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <iostream>
#include <string>
#include <complex>

struct DataStruct
{
    unsigned long long key1;
    std::complex<double> key2;
    std::string key3;
};

struct DelimiterIO
{
    char exp;
};

struct UllLitIO
{
    unsigned long long& ref;
};

struct ComplexIO
{
    std::complex<double>& ref;
};

struct StringIO
{
    std::string& ref;
};

std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
std::istream& operator>>(std::istream& in, UllLitIO&& dest);
std::istream& operator>>(std::istream& in, ComplexIO&& dest);
std::istream& operator>>(std::istream& in, StringIO&& dest);

std::istream& operator>>(std::istream& in, DataStruct& dest);
std::ostream& operator<<(std::ostream& out, const DataStruct& dest);

bool compareDataStruct(const DataStruct& lhs, const DataStruct& rhs);

#endif
