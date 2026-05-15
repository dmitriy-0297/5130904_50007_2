#include <cctype>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>

#include "CrossReference.h"

std::string normalizeWord(const std::string& source) {
    std::string result;
    result.reserve(source.size());
    for (char ch : source) {
        unsigned char uch = static_cast<unsigned char>(ch);
        if (std::isalpha(uch)) {
            result += static_cast<char>(std::tolower(uch));
        }
    }
    return result;
}

int indexFile(CrossReference& xref, const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("cannot open file: " + path);
    }
    std::string line;
    int lineNumber = 0;
    int occurrences = 0;
    while (std::getline(file, line)) {
        ++lineNumber;
        std::string current;
        for (std::size_t i = 0; i < line.size(); ++i) {
            unsigned char ch = static_cast<unsigned char>(line[i]);
            if (std::isalpha(ch)) {
                current += static_cast<char>(std::tolower(ch));
                continue;
            }
            if (!current.empty()) {
                xref.insert(current, lineNumber);
                ++occurrences;
                current.clear();
            }
        }
        if (!current.empty()) {
            xref.insert(current, lineNumber);
            ++occurrences;
        }
    }
    return occurrences;
}

int showFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("cannot open file: " + path);
    }
    std::string line;
    int lineNumber = 0;
    while (std::getline(file, line)) {
        ++lineNumber;
        std::cout << lineNumber << ": " << line << std::endl;
    }
    return lineNumber;
}

void printLineSet(const std::set<int>& lines) {
    bool first = true;
    for (int line : lines) {
        if (!first) {
            std::cout << ", ";
        }
        std::cout << line;
        first = false;
    }
}

void printAll(const CrossReference& xref) {
    if (xref.empty()) {
        std::cout << "(empty)" << std::endl;
        return;
    }
    for (const auto& entry : xref) {
        std::cout << entry.first << ": ";
        printLineSet(entry.second);
        std::cout << std::endl;
    }
}

void printHelp() {
    std::cout << "available commands:" << std::endl;
    std::cout << "  show <file>           print the text with line numbers"
              << std::endl;
    std::cout << "  load <file>           read a text file and index every word"
              << std::endl;
    std::cout << "  text                  print the last loaded file with line"
              << " numbers" << std::endl;
    std::cout << "  insert <word> <line>  add a single word occurrence"
              << std::endl;
    std::cout << "  search <word>         print 'word: lines' for a word"
              << std::endl;
    std::cout << "  remove <word>         delete a word from the index"
              << std::endl;
    std::cout << "  print                 print the whole cross-reference"
              << std::endl;
    std::cout << "  size                  print the number of unique words"
              << std::endl;
    std::cout << "  clear                 remove every entry" << std::endl;
    std::cout << "  help                  show this help message" << std::endl;
    std::cout << "press Ctrl+D (Linux) or Ctrl+Z then Enter (Windows) to exit."
              << std::endl;
}

void handleShow(std::istringstream& iss) {
    std::string path;
    if (!(iss >> path)) {
        std::cout << "usage: show <file>" << std::endl;
        return;
    }
    try {
        int total = showFile(path);
        std::cout << "(" << total << " line(s) printed)" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "error: " << e.what() << std::endl;
    }
}

void handleLoad(CrossReference& xref, std::string& loadedPath,
                std::istringstream& iss) {
    std::string path;
    if (!(iss >> path)) {
        std::cout << "usage: load <file>" << std::endl;
        return;
    }
    try {
        int count = indexFile(xref, path);
        loadedPath = path;
        std::cout << "loaded " << count << " word occurrence(s) from \""
                  << path << "\"." << std::endl;
    } catch (const std::exception& e) {
        std::cout << "error: " << e.what() << std::endl;
    }
}

void handleText(const std::string& loadedPath) {
    if (loadedPath.empty()) {
        std::cout << "no file has been loaded yet. use 'load <file>' first."
                  << std::endl;
        return;
    }
    try {
        int total = showFile(loadedPath);
        std::cout << "(" << total << " line(s) from \"" << loadedPath << "\")"
                  << std::endl;
    } catch (const std::exception& e) {
        std::cout << "error: " << e.what() << std::endl;
    }
}

void handleInsert(CrossReference& xref, std::istringstream& iss) {
    std::string rawWord;
    int line = 0;
    if (!(iss >> rawWord >> line)) {
        std::cout << "usage: insert <word> <line>" << std::endl;
        return;
    }
    std::string word = normalizeWord(rawWord);
    if (word.empty()) {
        std::cout << "error: word must contain at least one letter."
                  << std::endl;
        return;
    }
    try {
        xref.insert(word, line);
        std::cout << "inserted \"" << word << "\" at line " << line << "."
                  << std::endl;
    } catch (const std::exception& e) {
        std::cout << "error: " << e.what() << std::endl;
    }
}

void handleSearch(const CrossReference& xref, std::istringstream& iss) {
    std::string rawWord;
    if (!(iss >> rawWord)) {
        std::cout << "usage: search <word>" << std::endl;
        return;
    }
    std::string word = normalizeWord(rawWord);
    if (word.empty()) {
        std::cout << "error: word must contain at least one letter."
                  << std::endl;
        return;
    }
    try {
        if (!xref.contains(word)) {
            std::cout << "word \"" << word << "\" not found." << std::endl;
            return;
        }
        std::cout << word << ": ";
        printLineSet(xref.linesOf(word));
        std::cout << std::endl;
    } catch (const std::exception& e) {
        std::cout << "error: " << e.what() << std::endl;
    }
}

void handleRemove(CrossReference& xref, std::istringstream& iss) {
    std::string rawWord;
    if (!(iss >> rawWord)) {
        std::cout << "usage: remove <word>" << std::endl;
        return;
    }
    std::string word = normalizeWord(rawWord);
    if (word.empty()) {
        std::cout << "error: word must contain at least one letter."
                  << std::endl;
        return;
    }
    try {
        bool removed = xref.remove(word);
        if (removed) {
            std::cout << "removed \"" << word << "\"." << std::endl;
        } else {
            std::cout << "word \"" << word << "\" not found." << std::endl;
        }
    } catch (const std::exception& e) {
        std::cout << "error: " << e.what() << std::endl;
    }
}

int main() {
    CrossReference xref;
    std::string loadedPath;
    std::cout << "cross-reference console. type 'help' for commands."
              << std::endl;
    std::string line;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) {
            break;
        }
        std::istringstream iss(line);
        std::string command;
        if (!(iss >> command)) {
            continue;
        }
        if (command == "show") {
            handleShow(iss);
        } else if (command == "load") {
            handleLoad(xref, loadedPath, iss);
        } else if (command == "text") {
            handleText(loadedPath);
        } else if (command == "insert") {
            handleInsert(xref, iss);
        } else if (command == "search") {
            handleSearch(xref, iss);
        } else if (command == "remove") {
            handleRemove(xref, iss);
        } else if (command == "print") {
            printAll(xref);
        } else if (command == "size") {
            std::cout << xref.size() << " unique word(s)." << std::endl;
        } else if (command == "clear") {
            xref.clear();
            loadedPath.clear();
            std::cout << "cleared." << std::endl;
        } else if (command == "help") {
            printHelp();
        } else {
            std::cout << "unknown command: " << command
                      << ". type 'help' for the list." << std::endl;
        }
    }
    return 0;
}

