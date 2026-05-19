#ifndef INC_5130904_50007_2_CROSSREF_HPP
#define INC_5130904_50007_2_CROSSREF_HPP
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <iterator>
#include <map>
#include <map>
#include <map>
#include <map>

namespace polozhuk {
    class CrossReference {
        using map_ = std::map<std::string, std::vector<size_t>>;
        using iterator = map_::iterator;
        using const_iterator = map_::const_iterator;
    private:
        map_ my_map_;
        std::string clean_word(std::string word) {
            std::string temp = "";
            std::copy_if(word.cbegin(), word.cend(), std::back_inserter(temp),
                [](char c) {
                    if (std::isalpha(c)) {
                        return true;
                    }
                    return false;
                });
            std::transform(temp.begin(), temp.end(), temp.begin(),
                [](char c) {
                return std::tolower(c);
            });
            return temp;
        }

    public:

    };
};
#endif