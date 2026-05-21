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

namespace polozhuk
{
    class CrossReference
    {
    public:
        void build_index(const std::string& filepath);
        void print_index() const;
        void search_word(const std::string& text);
        void delete_word(const std::string& text);
        size_t count(const std::string& text);
        void clear()
        {
            my_map_.clear();
        }
        size_t words_count() const {
            return my_map_.size();
        }
    private:
        using map_ = std::map<std::string, std::vector<size_t>>;
        using iterator = map_::iterator;
        using const_iterator = map_::const_iterator;
        map_ my_map_;
        static std::string clean_word(std::string word);
    };
};

#endif
