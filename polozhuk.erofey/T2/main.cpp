#include "DataStruct.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>
#include <limits>

bool cmp(const DataStruct& a, const DataStruct& b){
    if (a.key1 != b.key1) {
        return a.key1 < b.key1;
    }
    if (a.key2 != b.key2) {
        return a.key2 < b.key2;
    }
    return a.key3.size() < b.key3.size();
}

int main(){
    std::vector< DataStruct > data;
    DataStruct tmp;
    while (!std::cin.eof()) {
        if (std::cin >> tmp) {
            data.push_back(tmp);
        } else if (!std::cin.eof()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    std::sort(std::begin(data), std::end(data), cmp);
    std::copy(
        std::begin(data),
        std::end(data),
        std::ostream_iterator< DataStruct >(std::cout, "\n")
    );
    return 0;
}

