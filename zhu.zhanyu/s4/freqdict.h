#ifndef FREQDICT_H
#define FREQDICT_H
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <exception>
class FileOpenException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Error: Cannot open file";
    }
};
class MemoryAllocationException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Error: Memory allocation failed";
    }
};
class FreqDict {
private:
    std::map<std::string, int> dict_;
    int totalWords_;
    bool isAlpha(char c) const;
    std::string toLower(const std::string& word) const;
    void processLine(const std::string& line);
public:
    FreqDict();
    void loadFile(const std::string& filename);
    int search(const std::string& word) const;
    void clear();
    int getUniqueWordCount() const;
    int getTotalWordCount() const;
    void printAll() const;
    void printTop(int n) const;
};
#endif
