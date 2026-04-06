#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <iostream>
#include <string>

namespace kulikov
{
  struct DataStruct
  {
    unsigned long long key1;
    unsigned long long key2;
    std::string key2Raw;
    std::string key3;
  };

  std::istream & operator>>(std::istream & in, DataStruct & data);
  std::ostream & operator<<(std::ostream & out, const DataStruct & data);

  struct DataStructComparator
  {
    bool operator()(const DataStruct & lhs, const DataStruct & rhs) const;
  };
}

#endif
