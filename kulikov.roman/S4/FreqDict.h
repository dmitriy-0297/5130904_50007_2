#ifndef FREQ_DICT_H
#define FREQ_DICT_H

#include <string>
#include <vector>
#include <map>

struct WordCount
{
    std::string word_;
    int         count_;
};

struct DictStats
{
    size_t unique_;
    int    total_;
    double average_;
};

class FreqDict
{
public:
    FreqDict()  = default;
    ~FreqDict() = default;

    void addWord(const std::string& word);
    int  findWord(const std::string& word) const;
    void deleteWord(const std::string& word);

    std::vector<WordCount> getTopThree() const;
    std::vector<WordCount> getAllSorted() const;
    DictStats              getStats()    const;

    void parseText(const std::string& text);
    void loadFromFile(const std::string& filename);
    void saveToFile(const std::string& filename) const;

    void   clear();
    size_t size() const;

private:
    std::map<std::string, int> dict_;
};

#endif
