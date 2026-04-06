#include <iostream>
#include <vector>
#include <algorithm>
#include "DataStruct.h"

int main()
{
    std::vector<DataStruct> records;
    DataStruct temp;

    while (std::cin >> temp)
    {
        records.push_back(temp);
    }

    if (records.empty())
    {
        std::cerr << "Looks like there is no supported record. Cannot determine input. Test skipped" << std::endl;
        return 0;
    }

    std::cout << "Atleast one supported record type" << std::endl;

    std::sort(records.begin(), records.end(), compareDataStruct);

    for (const auto& record : records)
    {
        std::cout << record << std::endl;
    }

    return 0;
}
