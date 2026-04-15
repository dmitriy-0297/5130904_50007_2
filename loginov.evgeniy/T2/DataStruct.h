#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <iostream>
#include <string>
#include <complex>

namespace loginov
{
  struct DataStruct
  {
    unsigned long long key1_;
    std::complex<double> key2_;
    std::string key3_;
  };

  std::istream & operator>>(std::istream & in, DataStruct & data);
  std::ostream & operator<<(std::ostream & out, const DataStruct & data);

  struct DataStructComparator
  {
    bool operator()(const DataStruct & lhs, const DataStruct & rhs) const;
  };
}
#endif
