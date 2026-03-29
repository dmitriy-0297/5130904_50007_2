#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include "DataStruct.h"

int main()
{
    std::vector< DataStruct > data;

    std::copy(
        std::istream_iterator< DataStruct >(std::cin),
        std::istream_iterator< DataStruct >(),
        std::back_inserter(data)
    );

    std::sort(std::begin(data), std::end(data), compareDataStruct);

    std::copy(
        std::begin(data),
        std::end(data),
        std::ostream_iterator< DataStruct >(std::cout, "\n")
    );

    return 0;
}
