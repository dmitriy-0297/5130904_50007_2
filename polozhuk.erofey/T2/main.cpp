#include "DataStruct.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>
bool cmp(const DataStruct& a, const DataStruct& b){
    if (a.key1 != b.key1) {
        return a.key1 < b.key1;
    }
    if (a.key2 != b.key2) {
        return a.key2 < b.key2;
    }
    return a.key3.size() < b.key3.size();
}
int main()
{
    std::vector< DataStruct > data;

    std::copy(
        std::istream_iterator< DataStruct >(std::cin),
        std::istream_iterator< DataStruct >(),
        std::back_inserter(data)
    );

    std::sort(std::begin(data), std::end(data), cmp);

    std::copy(
        std::begin(data),
        std::end(data),
        std::ostream_iterator< DataStruct >(std::cout, "\n")
    );

    return 0;
}

