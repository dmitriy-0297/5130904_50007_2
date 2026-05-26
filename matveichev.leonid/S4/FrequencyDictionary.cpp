#include "FrequencyDictionary.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <cctype>

std::string FrequencyDictionary::normalizeWord(const std::string& rawWord) {
    std::string result;
    result.reserve(rawWord.size());
    for (char character : rawWord) {
        if (std::isalpha(static_cast<unsigned char>(character))) {
            result += static_cast<char>(std::tolower(static_cast<unsigned char>(character)));
        }
    }
    return result;
}

void FrequencyDictionary::processStream(std::istream& inputStream) {
    constexpr size_t WORD_BUFFER_SIZE = 45;
    std::string word;
    word.reserve(WORD_BUFFER_SIZE);
    char character{};
    while (inputStream.get(character)) {
        bool isLatinLetter = std::isalpha(static_cast<unsigned char>(character));
        if (isLatinLetter) {
            word += static_cast<char>(std::tolower(static_cast<unsigned char>(character)));
        } else {
            if (!word.empty()) {
                ++map_[word];
                ++totalWordCount_;
                word.clear();
            }
        }
    }
    if (!word.empty()) {
        ++map_[word];
        ++totalWordCount_;
    }
}

void FrequencyDictionary::readText(const std::string& text) {
    std::istringstream inputStream(text);
    processStream(inputStream);
}

void FrequencyDictionary::readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    processStream(file);
}

void FrequencyDictionary::insert(const std::string& word) {
    std::string normalizedWord = normalizeWord(word);
    if (normalizedWord.empty()) {
        throw std::invalid_argument("INSERT: word contains no Latin letters: \"" + word + "\"");
    }
    ++map_[normalizedWord];
    ++totalWordCount_;
}

void FrequencyDictionary::remove(const std::string& word) {
    std::string normalizedWord = normalizeWord(word);
    auto iterator = map_.find(normalizedWord);
    if (iterator == map_.end()) {
        throw std::out_of_range("DELETE: word not found: \"" + normalizedWord + "\"");
    }
    totalWordCount_ -= static_cast<size_t>(iterator->second);
    map_.erase(iterator);
}

int FrequencyDictionary::search(const std::string& word) const {
    std::string normalizedWord = normalizeWord(word);
    auto iterator = map_.find(normalizedWord);
    if (iterator == map_.end()) {
        return -1;
    }
    return iterator->second;
}

std::vector<std::pair<std::string, int>>
FrequencyDictionary::getTopFrequentWords(size_t n) const {
    std::vector<std::pair<std::string, int>> entries(map_.begin(), map_.end());
    std::sort(entries.begin(), entries.end(),
        [](const std::pair<std::string, int>& lhs, const std::pair<std::string, int>& rhs) {
            if (lhs.second != rhs.second) {
                return lhs.second > rhs.second;
            }
            return lhs.first < rhs.first;
        });
    if (n > entries.size()) {
        n = entries.size();
    }
    return {entries.begin(), entries.begin() + static_cast<std::ptrdiff_t>(n)};
}

std::vector<std::pair<std::string, int>>
FrequencyDictionary::getAllWordsSorted() const {
    std::vector<std::pair<std::string, int>> entries(map_.begin(), map_.end());
    std::sort(entries.begin(), entries.end(),
        [](const std::pair<std::string, int>& lhs, const std::pair<std::string, int>& rhs) {
            return lhs.first < rhs.first;
        });
    return entries;
}

size_t FrequencyDictionary::getUniqueWordCount() const {
    return map_.size();
}

size_t FrequencyDictionary::getTotalWordCount() const {
    return totalWordCount_;
}

void FrequencyDictionary::clear() {
    map_.clear();
    totalWordCount_ = 0;
}
