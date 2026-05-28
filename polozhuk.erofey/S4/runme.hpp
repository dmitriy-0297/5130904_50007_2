#ifndef INC_5130904_50007_2_RUNME_HPP
#define INC_5130904_50007_2_RUNME_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include "crossref.hpp"

namespace polozhuk {
    enum class Doing {
        LOAD,
        SEARCH,
        PRINT,
        DELETE,
        COUNT,
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
            {"count", Doing::COUNT},
            {"delete", Doing::DELETE},
            {"words", Doing::WORDS},
            {"clear", Doing::CLEAR},
        };

        std::cout << "So we can do: load <filename>, search <word>, print, words, clear" << std::endl;
        std::cout << "Press Ctrl+D to exit." << std::endl;
        while (true) {
            std::cout << " > ";
            std::string line;

            if (!std::getline(std::cin, line)) {
                std::cout << "EOF received. Exiting...\n";
                break;
            }

            std::istringstream iss(line);
            std::string cmd_stream;
            if (!(iss >> cmd_stream)) {
                continue;
            }

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
                    } else {
                        std::cout << "wrong word\n";
                    }
                    break;
                }
                case Doing::DELETE: {
                    std::string doing_line;
                    if (iss >> doing_line) {
                        my_crossref.delete_word(doing_line);
                    }
                    break;
                }
                case Doing::COUNT: {
                    std::string doing_line;
                    if (iss >> doing_line) {
                        std::cout << "Word " << doing_line << " found "
                                << my_crossref.count(doing_line) << " times.\n";
                    } else {
                        std::cout << "wrong word\n";
                    }
                    break;
                }
                case Doing::PRINT: {
                    my_crossref.print_index();
                    break;
                }
                case Doing::WORDS: {
                    std::cout << "Unique words count: " << my_crossref.words_count() << "\n";
                    break;
                    break;
                }
                case Doing::CLEAR: {
                    my_crossref.clear();
                    break;
                }
                case Doing::UNKNOWN:
                default:
                    std::cout << "Unknown command.\n";
                    std::cin.clear();
                    break;
            }
        }
    }
}


#endif // INC_5130904_50007_2_RUNME_HPP

