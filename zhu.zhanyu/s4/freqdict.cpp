#include "freqdict.h"
FreqDict::FreqDict() : totalWords_(0) {}
bool FreqDict::isAlpha(char c) const {
    return std::isalpha(static_cast<unsigned char>(c));
}
std::string FreqDict::toLower(const std::string& word) const {
    std::string result = word;
    for (char& c : result) {
        c = std::tolower(static_cast<unsigned char>(c));
    }
    return result;
}
void FreqDict::processLine(const std::string& line) {
    std::string word;
    for (char c : line) {
        if (isAlpha(c)) {
            word += c;
        }
        else {
            if (!word.empty()) {
                word = toLower(word);
                try {
                    dict_[word]++;
                }
                catch (const std::bad_alloc&) {
                    throw MemoryAllocationException();
                }
                totalWords_++;
                word.clear();
            }
        }
    }
    if (!word.empty()) {
        word = toLower(word);
        try {
            dict_[word]++;
        }
        catch (const std::bad_alloc&) {
            throw MemoryAllocationException();
        }
        totalWords_++;
    }
}
void FreqDict::loadFile(const std::string& filename) {
    std::ifstream file;
    try {
        file.open(filename);
        if (!file.is_open()) {
            throw FileOpenException();
        }
        std::string line;
        while (std::getline(file, line)) {
            processLine(line);
        }
        file.close();
    }
    catch (const MemoryAllocationException& e) {
        std::cerr << e.what() << std::endl;
        clear();
        throw;
    }
    catch (const FileOpenException& e) {
        std::cerr << e.what() << std::endl;
        if (file.is_open()) file.close();
        throw;
    }
    catch (const std::exception& e) {
        if (file.is_open()) file.close();
        throw;
    }
}
int FreqDict::search(const std::string& word) const {
    std::string lowerWord = toLower(word);
    auto it = dict_.find(lowerWord);
    if (it != dict_.end()) {
        return it->second;
    }
    return 0;
}
void FreqDict::clear() {
    dict_.clear();
    totalWords_ = 0;
}
int FreqDict::getUniqueWordCount() const {
    return static_cast<int>(dict_.size());
}
int FreqDict::getTotalWordCount() const {
    return totalWords_;
}
void FreqDict::printAll() const {
    if (dict_.empty()) {
        std::cout << "Dictionary is empty." << std::endl;
        return;
    }
    std::cout << "\n=== All words ===" << std::endl;
    for (const auto& pair : dict_) {
        std::cout << "  " << pair.first << " : " << pair.second << std::endl;
    }
    std::cout << "======================" << std::endl;
}
void FreqDict::printTop(int n) const {
    if (dict_.empty()) {
        std::cout << "Dictionary is empty." << std::endl;
        return;
    }
    std::vector<std::pair<std::string, int>> vec;
    try {
        vec.reserve(dict_.size());
        for (const auto& pair : dict_) {
            vec.push_back(pair);
        }
    }
    catch (const std::bad_alloc&) {
        throw MemoryAllocationException();
    }
    std::sort(vec.begin(), vec.end(),
        [](const std::pair<std::string, int>& a,
            const std::pair<std::string, int>& b) {
                if (a.second != b.second) {
                    return a.second > b.second;
                }
                return a.first < b.first;
        });
    int count = std::min(n, static_cast<int>(vec.size()));
    std::cout << "\n=== Top " << count << " words ===" << std::endl;
    for (int i = 0; i < count; ++i) {
        std::cout << "  " << (i + 1) << ". " << vec[i].first
            << " : " << vec[i].second << std::endl;
    }
    std::cout << "======================" << std::endl;
}
