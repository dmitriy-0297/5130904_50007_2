#include "crossref.hpp"

namespace polozhuk
{
    enum class Doing {
        LOAD,
        SEARCH,
        PRINT,
        WORDS,
        CLEAR,
        UNKNOWN
    };
    void run_me() {
        CrossReference my_crossref;
        const std::map<std::string, Doing> doing_map = {
            {"load", Doing::LOAD},
            {"search", Doing::SEARCH},
            {"print", Doing::PRINT},
            {"words", Doing::WORDS},
            {"clear", Doing::CLEAR},
        };
        std::cout << "So we can do: load , search , "
                     "print, words, clear" << std::endl;
        std::cout << "press Ctrl+D (Linux) or Ctrl+Z then Enter (Windows) to exit."
              << std::endl;
        while (true) {
            std::cout << " > ";
            std::string line;
            if (!std::getline(std::cin, line)) {
                std::cout << "EOF received. Exiting...\n";
                break;
            }

            if (line.empty()) continue;
            std::istringstream iss(line);
            std::string cmd_stream;
            iss >> cmd_stream;

            Doing doing = Doing::UNKNOWN;
            auto it = doing_map.find(cmd_stream);
            if (it != doing_map.end()) {
                doing = it->second;
            }

            switch (doing) {
                case Doing::LOAD: {
                    std::string doing_line;
                    if (iss >> doing_line) {
                        my_crossref.build_index(doing_line);
                    } else {
                        std::cout << "wrong filename\n";
                    }
                    break;
                }
                case Doing::SEARCH: {
                    std::string doing_line;
                    if (iss >> doing_line) {
                        my_crossref.search_word(doing_line);
                    }
                    else {
                        std::cout << "wrong word\n";
                    }
                    break;
                }
                case Doing::PRINT: {
                    my_crossref.print_index();
                    break;
                }
                case Doing::WORDS: {
                    my_crossref.words_count();
                    break;
                }
                case Doing::CLEAR: {
                    my_crossref.clear();
                    break;

                }
                case Doing::UNKNOWN:
                default:
                    std::cout << "Unknown command.\n";
                    break;
            }
        }
    }
};

