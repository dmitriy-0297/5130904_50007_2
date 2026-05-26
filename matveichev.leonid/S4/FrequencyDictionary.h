#ifndef FREQUENCYDICTIONARY_H
#define FREQUENCYDICTIONARY_H

#include <string>
#include <vector>
#include <utility>
#include <istream>
#include <unordered_map>

class FrequencyDictionary {
public:
    FrequencyDictionary() = default;

    void readText(const std::string& text);
    void readFile(const std::string& filename);
    void insert(const std::string& word);
    void remove(const std::string& word);
    int search(const std::string& word) const;
    std::vector<std::pair<std::string, int>> getTopFrequentWords(size_t n = 3) const;
    std::vector<std::pair<std::string, int>> getAllWordsSorted() const;

    size_t getUniqueWordCount() const;
    size_t getTotalWordCount() const;
    void clear();

    static std::string normalizeWord(const std::string& rawWord);

private:
    std::unordered_map<std::string, int> map_;
    size_t totalWordCount_ = 0;

    void processStream(std::istream& inputStream);
};

#endif
