#ifndef RUNME_HPP
#define RUNME_HPP
#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <cctype>
#include "freqdict.h"
inline std::string toLowerCmd(const std::string& str) {
    std::string result = str;
    for (char& c : result) {
        c = std::tolower(static_cast<unsigned char>(c));
    }
    return result;
}
enum class Command {
    LOAD,
    SEARCH,
    TOP,
    ALL,
    STATS,
    CLEAR,
    HELP,
    EXIT,
    UNKNOWN
};
inline void showHelp() {
    std::cout << "\n=== Available Commands ===" << std::endl;
    std::cout << "  load <filename>   - Load text file and count words"
        << std::endl;
    std::cout << "  search <word>     - Search frequency of a word"
        << std::endl;
    std::cout << "  top <n>           - Show top n most frequent words"
        << std::endl;
    std::cout << "  all               - Show all words with frequencies"
        << std::endl;
    std::cout << "  stats             - Show statistics (unique/total words)"
        << std::endl;
    std::cout << "  clear             - Clear all data" << std::endl;
    std::cout << "  help              - Show this help message" << std::endl;
    std::cout << "  exit              - Exit the program" << std::endl;
    std::cout << "==========================" << std::endl;
}
inline void run_me() {
    FreqDict dict;
    std::string line;
    const std::map<std::string, Command> cmd_map = {
        {"load",   Command::LOAD},
        {"search", Command::SEARCH},
        {"top",    Command::TOP},
        {"all",    Command::ALL},
        {"stats",  Command::STATS},
        {"clear",  Command::CLEAR},
        {"help",   Command::HELP},
        {"exit",   Command::EXIT}
    };
    std::cout << "=== Word Frequency Dictionary (STL version) ==="
        << std::endl;
    std::cout << "Type 'help' to see available commands." << std::endl;
    std::cout << "==============================================="
        << std::endl;
    while (true) {
        std::cout << "\n> ";
        if (!std::getline(std::cin, line)) {
            std::cout << "\nEOF received. Exiting..." << std::endl;
            break;
        }
        if (line.empty()) continue;
        std::istringstream iss(line);
        std::string cmd_str;
        iss >> cmd_str;
        cmd_str = toLowerCmd(cmd_str);
        Command cmd = Command::UNKNOWN;
        auto it = cmd_map.find(cmd_str);
        if (it != cmd_map.end()) {
            cmd = it->second;
        }
        switch (cmd) {
        case Command::LOAD: {
            std::string filename;
            if (iss >> filename) {
                try {
                    dict.loadFile(filename);
                    std::cout << "Loaded file: " << filename << std::endl;
                    std::cout << "Unique words: "
                        << dict.getUniqueWordCount() << std::endl;
                    std::cout << "Total words: "
                        << dict.getTotalWordCount() << std::endl;
                }
                catch (const FileOpenException& e) {
                    std::cerr << e.what() << std::endl;
                }
                catch (const MemoryAllocationException& e) {
                    std::cerr << e.what() << std::endl;
                    dict.clear();
                }
                catch (const std::exception& e) {
                    std::cerr << "Error: " << e.what() << std::endl;
                }
            }
            else {
                std::cout << "Usage: load <filename>" << std::endl;
            }
            break;
        }
        case Command::SEARCH: {
            std::string word;
            if (iss >> word) {
                int freq = dict.search(word);
                if (freq > 0) {
                    std::cout << "Word '" << word << "' appears "
                        << freq << " time(s)." << std::endl;
                }
                else {
                    std::cout << "Word '" << word << "' not found."
                        << std::endl;
                }
            }
            else {
                std::cout << "Usage: search <word>" << std::endl;
            }
            break;
        }
        case Command::TOP: {
            int n;
            if (iss >> n && n > 0) {
                try {
                    dict.printTop(n);
                }
                catch (const MemoryAllocationException& e) {
                    std::cerr << e.what() << std::endl;
                }
            }
            else {
                std::cout << "Usage: top <positive_number>" << std::endl;
            }
            break;
        }
        case Command::ALL:
            try {
                dict.printAll();
            }
            catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }
            break;
        case Command::STATS:
            std::cout << "=== Statistics ===" << std::endl;
            std::cout << "  Unique words: " << dict.getUniqueWordCount()
                << std::endl;
            std::cout << "  Total words:  " << dict.getTotalWordCount()
                << std::endl;
            std::cout << "=================" << std::endl;
            break;
        case Command::CLEAR:
            dict.clear();
            std::cout << "Dictionary cleared." << std::endl;
            break;
        case Command::HELP:
            showHelp();
            break;
        case Command::EXIT:
            std::cout << "Goodbye!" << std::endl;
            return;
        case Command::UNKNOWN:
        default:
            std::cout << "Unknown command: " << cmd_str << std::endl;
            std::cout << "Type 'help' to see available commands."
                << std::endl;
            std::cin.clear();
            break;
        }
    }
}
#endif
