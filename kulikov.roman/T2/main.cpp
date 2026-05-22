#include "DataStruct.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

int main()
{
  std::vector< kulikov::DataStruct > data;

  while (!std::cin.eof())
  {
    kulikov::DataStruct tmp{};
    if (std::cin >> tmp)
    {
      data.push_back(tmp);
    }
    else
    {
      std::cin.clear();
      std::string skip;
      std::getline(std::cin, skip);
    }
  }

  std::sort(data.begin(), data.end(), kulikov::DataStructComparator{});

  std::copy(
    data.cbegin(),
    data.cend(),
    std::ostream_iterator< kulikov::DataStruct >(std::cout, "\n")
  );

  return 0;
}
