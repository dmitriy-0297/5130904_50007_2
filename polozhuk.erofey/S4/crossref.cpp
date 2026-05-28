#include "crossref.hpp"

#include <numeric>

std::string polozhuk::CrossReference::clean_word(std::string word)
{
    std::string temp = "";
    std::copy_if(word.cbegin(), word.cend(), std::back_inserter(temp),
        [](char c) {
            return std::isalpha(c);
        });
    std::transform(temp.begin(), temp.end(), temp.begin(),
        [](char c) {
        return std::tolower(c);
    });
    return temp;
}
void polozhuk::CrossReference::search_word(const std::string& text)
{
    auto my_text = my_map_.find(clean_word(text));
    if (my_text != my_map_.end()) {
        std::copy(my_text->second.cbegin(), my_text->second.cend(),
            std::ostream_iterator<size_t>(std::cout, ", "));
        std::cout << std::endl;
        return;
    }
    std::cout << "nothing found"<< std::endl;
    return;
}
void polozhuk::CrossReference::print_index() const
{
    if (my_map_.empty()) {
        std::cout << "empty map" << std::endl;
        return;
    }
    std::for_each(my_map_.cbegin(), my_map_.cend(),
        [](const std::pair<const std::string, std::vector<size_t>> &item) {
        std::cout << item.first << " -> strings: ";
        std::copy(item.second.cbegin(),item.second.cend(),
                  std::ostream_iterator<size_t>(std::cout, " ")
        );
        std::cout << std::endl;
    });
}

void polozhuk::CrossReference::build_index(const std::string& filepath)
{
    std::ifstream ifs(filepath);
    std::string line;
    if (!ifs.is_open()) {
        std::cerr << "Error opening";
        throw std::runtime_error("Error opening");
    }
    size_t index = 0;
    while (std::getline(ifs, line)) {
        ++index;
        std::istringstream iss(line);
        using istrit = std::istream_iterator<std::string>;
        std::for_each(istrit(iss), istrit(),
        [this, index](const std::string& s) {
            std::string cword = clean_word(s);
            if (!cword.empty()) {
                my_map_[cword].push_back(index);
            }
        });
    }
}

void polozhuk::CrossReference::delete_word(const std::string& text) {
    auto my_text = my_map_.find(clean_word(text));
    if (my_text != my_map_.end()) {
        my_map_.erase(my_text);
    }
    else {
        std::cout << "nothing found"<< std::endl;
    }
    return;
}

size_t polozhuk::CrossReference::count(const std::string& text) const{
    if (my_map_.empty()) {
        std::cout << "empty map\n";
        return 0;
    }
    auto my_text = my_map_.find(clean_word(text));
    if (my_text != my_map_.end()) {
        return my_text->second.size();
    }
    return 0;
}
