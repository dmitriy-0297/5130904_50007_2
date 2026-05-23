#include "FreqDict.h"
#include "constants.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <numeric>
#include <stdexcept>

void FreqDict::addWord(const std::string& word)
{
    if (word.empty()) throw std::invalid_argument(EMPTY_WORD_ERR);
    ++dict_[word];
}

int FreqDict::findWord(const std::string& word) const
{
    if (word.empty()) throw std::invalid_argument(EMPTY_WORD_ERR);
    auto it = dict_.find(word);
    return (it == dict_.end()) ? 0 : it->second;
}

void FreqDict::deleteWord(const std::string& word)
{
    if (word.empty()) throw std::invalid_argument(EMPTY_WORD_ERR);
    auto it = dict_.find(word);
    if (it == dict_.end()) throw std::invalid_argument(WORD_NOT_FOUND_ERR);
    dict_.erase(it);
}

std::vector<WordCount> FreqDict::getTopThree() const
{
    if (dict_.empty()) throw std::runtime_error(DICT_EMPTY_ERR);

    std::vector<WordCount> entries;
    entries.reserve(dict_.size());
    for (const auto& kv : dict_)
        entries.push_back({kv.first, kv.second});

    const size_t limit = std::min<size_t>(3, entries.size());
    std::partial_sort(entries.begin(), entries.begin() + limit, entries.end(),
        [](const WordCount& a, const WordCount& b) {
            return a.count_ > b.count_;
        });
    entries.resize(limit);
    return entries;
}

std::vector<WordCount> FreqDict::getAllSorted() const
{
    std::vector<WordCount> entries;
    entries.reserve(dict_.size());
    for (const auto& kv : dict_)
        entries.push_back({kv.first, kv.second});

    std::sort(entries.begin(), entries.end(),
        [](const WordCount& a, const WordCount& b) {
            return a.count_ != b.count_ ? a.count_ > b.count_
                                        : a.word_  < b.word_;
        });
    return entries;
}

DictStats FreqDict::getStats() const
{
    DictStats s{};
    s.unique_ = dict_.size();
    s.total_  = std::accumulate(dict_.begin(), dict_.end(), 0,
        [](int sum, const std::map<std::string, int>::value_type& kv) {
            return sum + kv.second;
        });
    s.average_ = (s.unique_ == 0) ? 0.0
                                   : static_cast<double>(s.total_) / s.unique_;
    return s;
}

void FreqDict::parseText(const std::string& text)
{
    std::string current;
    for (unsigned char ch : text)
    {
        if (std::isalpha(ch))
            current += static_cast<char>(std::tolower(ch));
        else if (!current.empty())
        {
            addWord(current);
            current.clear();
        }
    }
    if (!current.empty()) addWord(current);
}

void FreqDict::loadFromFile(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Cannot open file: " + filename);
    std::string line;
    while (std::getline(file, line))
        parseText(line);
}

void FreqDict::saveToFile(const std::string& filename) const
{
    std::ofstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Cannot open file for writing: " + filename);
    for (const auto& kv : dict_)
        file << kv.first << ' ' << kv.second << '\n';
}

void FreqDict::clear()
{
    dict_.clear();
}

size_t FreqDict::size() const
{
    return dict_.size();
}
