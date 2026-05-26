#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <stdexcept>
#include "FrequencyDictionary.h"

constexpr size_t SEPARATOR_LENGTH = 52;

static void printSeparator() {
    std::cout << std::string(SEPARATOR_LENGTH, '-') << "\n";
}

static void printWordTable(const std::vector<std::pair<std::string, int>>& words) {
    if (words.empty()) {
        std::cout << "  (empty)\n";
        return;
    }
    printSeparator();
    std::cout << std::left << std::setw(6) << "  #"
              << std::setw(24) << "Word"
              << std::setw(10) << "Count" << "\n";
    printSeparator();
    for (size_t index = 0; index < words.size(); ++index) {
        std::cout << "  " << std::left << std::setw(4) << (index + 1)
                  << std::setw(24) << words[index].first
                  << std::setw(10) << words[index].second << "\n";
    }
    printSeparator();
}

static void printHelp() {
    std::cout
        << "\n+----------------------------------------------------+\n"
        << "|        Frequency Dictionary — Commands             |\n"
        << "+----------------------------------------------------+\n"
        << "|  insert <word>          Insert / increment word    |\n"
        << "|  delete <word>          Remove word completely     |\n"
        << "|  search <word>          Show frequency of a word   |\n"
        << "|  top [N]                Top N frequent words (3)   |\n"
        << "|  list                   List all words (A-Z)       |\n"
        << "|  load <text...>         Parse text into dict       |\n"
        << "|  file <filename>        Load text from file        |\n"
        << "|  stats                  Show dictionary stats      |\n"
        << "|  clear                  Clear the dictionary       |\n"
        << "|  help                   Show this help             |\n"
        << "+----------------------------------------------------+\n\n";
}

static void cmdInsert(FrequencyDictionary& dictionary, const std::string& argument) {
    if (argument.empty()) {
        std::cout << "Usage: insert <word>\n";
        return;
    }
    try {
        dictionary.insert(argument);
        int count = dictionary.search(FrequencyDictionary::normalizeWord(argument));
        std::cout << "OK. \"" << FrequencyDictionary::normalizeWord(argument)
                  << "\" => " << count << "\n";
    } catch (const std::exception& error) {
        std::cout << "Error: " << error.what() << "\n";
    }
}

static void cmdDelete(FrequencyDictionary& dictionary, const std::string& argument) {
    if (argument.empty()) {
        std::cout << "Usage: delete <word>\n";
        return;
    }
    try {
        dictionary.remove(argument);
        std::cout << "OK. \"" << FrequencyDictionary::normalizeWord(argument)
                  << "\" removed.\n";
    } catch (const std::exception& error) {
        std::cout << "Error: " << error.what() << "\n";
    }
}

static void cmdSearch(const FrequencyDictionary& dictionary, const std::string& argument) {
    if (argument.empty()) {
        std::cout << "Usage: search <word>\n";
        return;
    }
    std::string normalizedWord = FrequencyDictionary::normalizeWord(argument);
    int count = dictionary.search(normalizedWord);
    if (count < 0) {
        std::cout << "\"" << normalizedWord << "\" — not found.\n";
    } else {
        std::cout << "\"" << normalizedWord << "\" => " << count << " occurrence(s)\n";
    }
}

static void cmdTop(const FrequencyDictionary& dictionary, const std::string& argument) {
    constexpr size_t DEFAULT_TOP_N = 3;
    size_t topCount = DEFAULT_TOP_N;
    if (!argument.empty()) {
        try {
            int temporaryValue = std::stoi(argument);
            if (temporaryValue <= 0) {
                std::cout << "Error: N must be a positive integer.\n";
                return;
            }
            topCount = static_cast<size_t>(temporaryValue);
        } catch (...) {
            std::cout << "Error: N must be a positive integer.\n";
            return;
        }
    }
    std::vector<std::pair<std::string, int>> topWords = dictionary.getTopFrequentWords(topCount);
    std::cout << "Top " << topCount << " word(s):\n";
    printWordTable(topWords);
}

static void cmdList(const FrequencyDictionary& dictionary) {
    std::vector<std::pair<std::string, int>> allWords = dictionary.getAllWordsSorted();
    std::cout << "All words (A-Z), total unique: " << dictionary.getUniqueWordCount() << "\n";
    printWordTable(allWords);
}

static void cmdLoad(FrequencyDictionary& dictionary, const std::string& text) {
    if (text.empty()) {
        std::cout << "Usage: load <text...>\n";
        return;
    }
    size_t countBefore = dictionary.getTotalWordCount();
    dictionary.readText(text);
    size_t addedCount = dictionary.getTotalWordCount() - countBefore;
    std::cout << "OK. Added " << addedCount << " word(s). "
              << "Total unique: " << dictionary.getUniqueWordCount() << "\n";
}

static void cmdFile(FrequencyDictionary& dictionary, const std::string& filename) {
    if (filename.empty()) {
        std::cout << "Usage: file <filename>\n";
        return;
    }
    try {
        size_t countBefore = dictionary.getTotalWordCount();
        dictionary.readFile(filename);
        size_t addedCount = dictionary.getTotalWordCount() - countBefore;
        std::cout << "OK. Read " << addedCount << " word(s) from \"" << filename << "\". "
                  << "Total unique: " << dictionary.getUniqueWordCount() << "\n";
    } catch (const std::exception& error) {
        std::cout << "Error: " << error.what() << "\n";
    }
}

static void cmdStats(const FrequencyDictionary& dictionary) {
    std::cout << "Dictionary statistics:\n";
    printSeparator();
    std::cout << "  Unique words : " << dictionary.getUniqueWordCount() << "\n";
    std::cout << "  Total words  : " << dictionary.getTotalWordCount() << "\n";
    printSeparator();
    if (dictionary.getUniqueWordCount() > 0) {
        std::vector<std::pair<std::string, int>> topWords = dictionary.getTopFrequentWords(1);
        std::cout << "  Most frequent: \"" << topWords[0].first
                  << "\" (" << topWords[0].second << " times)\n";
        printSeparator();
    }
}

static void cmdClear(FrequencyDictionary& dictionary) {
    dictionary.clear();
    std::cout << "OK. Dictionary cleared.\n";
}

int main() {
    FrequencyDictionary dictionary;

    std::cout << "Frequency Dictionary  [std::unordered_map]\n";
    std::cout << "Type 'help' for command list. Ctrl+Z (Windows) or Ctrl+D (Linux) to quit.\n\n";

    std::string line;
    while (true) {
        std::cout << "> ";
        std::cout.flush();

        if (!std::getline(std::cin, line)) {
            break;
        }

        size_t startIndex = line.find_first_not_of(" \t");
        if (startIndex == std::string::npos) {
            continue;
        }
        line = line.substr(startIndex);
        if (line.empty()) {
            continue;
        }

        std::istringstream inputStream(line);
        std::string command;
        inputStream >> command;
        std::string arguments;
        std::getline(inputStream, arguments);
        if (!arguments.empty() && arguments[0] == ' ') {
            arguments = arguments.substr(1);
        }

        if (command == "insert") {
            cmdInsert(dictionary, arguments);
        } else if (command == "delete") {
            cmdDelete(dictionary, arguments);
        } else if (command == "search") {
            cmdSearch(dictionary, arguments);
        } else if (command == "top") {
            cmdTop(dictionary, arguments);
        } else if (command == "list") {
            cmdList(dictionary);
        } else if (command == "load") {
            cmdLoad(dictionary, arguments);
        } else if (command == "file") {
            cmdFile(dictionary, arguments);
        } else if (command == "stats") {
            cmdStats(dictionary);
        } else if (command == "clear") {
            cmdClear(dictionary);
        } else if (command == "help") {
            printHelp();
        } else {
            std::cout << "Unknown command: \"" << command
                      << "\". Type 'help' for the list of commands.\n";
        }
    }

    std::cout << "\nGoodbye.\n";
    return 0;
}
