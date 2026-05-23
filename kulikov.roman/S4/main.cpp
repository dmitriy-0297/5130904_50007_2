#include "FreqDict.h"

#include <cassert>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#if defined(__unix__) || defined(__APPLE__)
#include <unistd.h>
static void handleSigtstp(int)
{
    std::cout << "\nBye.\n";
    std::exit(0);
}
#endif

static void printHelp()
{
    std::cout <<
        "Commands:\n"
        "  add <word>      – add word (increment counter by 1)\n"
        "  find <word>     – show word frequency (0 if not found)\n"
        "  delete <word>   – remove word from dictionary\n"
        "  top             – show top-3 most frequent words\n"
        "  load <file>     – load and parse text from file\n"
        "  parse <text>    – parse given line as text\n"
        "  size            – show number of unique words\n"
        "  clear           – clear the dictionary\n"
        "  help            – show this message\n"
        "  stats           – show statistics (total, unique, average frequency)\n"
        "  (EOF to exit)\n";
}

static void runCommand(FreqDict& dict, const std::string& line)
{
    std::istringstream iss(line);
    std::string cmd;
    if (!(iss >> cmd)) return;

    try
    {
        if (cmd == "add")
        {
            std::string word;
            if (!(iss >> word)) { std::cout << "Usage: add <word>\n"; return; }
            dict.addWord(word);
            std::cout << "Added \"" << word << "\": " << dict.findWord(word) << "\n";
        }
        else if (cmd == "find")
        {
            std::string word;
            if (!(iss >> word)) { std::cout << "Usage: find <word>\n"; return; }
            std::cout << "\"" << word << "\": " << dict.findWord(word) << "\n";
        }
        else if (cmd == "delete")
        {
            std::string word;
            if (!(iss >> word)) { std::cout << "Usage: delete <word>\n"; return; }
            dict.deleteWord(word);
            std::cout << "Deleted \"" << word << "\"\n";
        }
        else if (cmd == "top")
        {
            auto top = dict.getTopThree();
            std::cout << "Top-" << top.size() << ":\n";
            for (size_t i = 0; i < top.size(); ++i)
                std::cout << "  " << (i + 1) << ". \""
                          << top[i].word_ << "\" – " << top[i].count_ << "\n";
        }
        else if (cmd == "load")
        {
            std::string filename;
            if (!(iss >> filename)) { std::cout << "Usage: load <file>\n"; return; }
            size_t before = dict.size();
            dict.loadFromFile(filename);
            std::cout << "Loaded \"" << filename << "\". "
                      << "Unique words: " << dict.size()
                      << " (+" << (dict.size() - before) << " new)\n";
        }
        else if (cmd == "parse")
        {
            std::string rest;
            std::getline(iss, rest);
            if (!rest.empty() && rest[0] == ' ') rest.erase(0, 1);
            if (rest.empty()) { std::cout << "Usage: parse <text>\n"; return; }
            size_t before = dict.size();
            dict.parseText(rest);
            std::cout << "Parsed. Unique words: " << dict.size()
                      << " (+" << (dict.size() - before) << " new)\n";
        }
        else if (cmd == "size")
        {
            std::cout << "Unique words: " << dict.size() << "\n";
        }
        else if (cmd == "clear")
        {
            dict.clear();
            std::cout << "Dictionary cleared.\n";
        }
        else if (cmd == "help")
        {
            printHelp();
        }
        else if (cmd == "stats")
        {
            DictStats s = dict.getStats();
            std::cout << std::fixed << std::setprecision(2)
                      << "Statistics:\n"
                      << "  Unique words    : " << s.unique_  << "\n"
                      << "  Total words     : " << s.total_   << "\n"
                      << "  Average freq.   : " << s.average_ << "\n";
        }
        else
        {
            std::cout << "Unknown command: \"" << cmd << "\". Type 'help' for the list.\n";
        }
    }
    catch (const std::exception& e)
    {
        std::cout << "Error: " << e.what() << "\n";
    }
}


static bool containsWC(const std::vector<WordCount>& v,
                        const std::string& w, int c)
{
    for (const auto& wc : v)
        if (wc.word_ == w && wc.count_ == c) return true;
    return false;
}

static void runTests()
{
    std::cout << "=== Running tests ===\n";

    {
        FreqDict d;
        d.addWord("hello"); d.addWord("hello"); d.addWord("world");
        assert(d.findWord("hello") == 2);
        assert(d.findWord("world") == 1);
        assert(d.findWord("none")  == 0);
        assert(d.size() == 2);
        std::cout << "1. Basic add/find         OK\n";
    }

    {
        FreqDict d;
        d.parseText("The THE the");
        assert(d.findWord("the") == 3);
        assert(d.size() == 1);
        d.parseText("Hello, world! Hello.");
        assert(d.findWord("hello") == 2);
        assert(d.findWord("world") == 1);
        std::cout << "2. parseText / lowercase  OK\n";
    }

    {
        FreqDict d;
        d.addWord("apple"); d.addWord("banana");
        d.deleteWord("apple");
        assert(d.findWord("apple") == 0);
        assert(d.size() == 1);
        bool caught = false;
        try { d.deleteWord("missing"); } catch (const std::invalid_argument&) { caught = true; }
        assert(caught);
        std::cout << "3. deleteWord             OK\n";
    }

    {
        FreqDict d;
        d.parseText("world world world world world "
                    "hello hello hello alpha alpha alpha "
                    "low low beta gamma the the are is am");
        auto top = d.getTopThree();
        assert(top.size() == 3);
        assert(containsWC(top, "world", 5));
        assert(containsWC(top, "hello", 3));
        assert(containsWC(top, "alpha", 3));
        std::cout << "4. getTopThree            OK\n";
    }

    {
        FreqDict empty;
        bool caught = false;
        try { empty.getTopThree(); } catch (const std::runtime_error&) { caught = true; }
        assert(caught);

        FreqDict one; one.addWord("only");
        auto t1 = one.getTopThree();
        assert(t1.size() == 1 && t1[0].word_ == "only");

        FreqDict two; two.parseText("apple banana apple");
        auto t2 = two.getTopThree();
        assert(t2.size() == 2);
        std::cout << "5. getTopThree edge cases OK\n";
    }

    {
        FreqDict d;
        d.parseText("one two three");
        d.clear();
        assert(d.size() == 0);
        d.parseText("four five");
        assert(d.size() == 2);
        assert(d.findWord("one") == 0);
        std::cout << "6. clear / rebuild        OK\n";
    }

    {
        const char* fname = "s4_test.txt";
        { std::ofstream f(fname); f << "cat dog cat\ndog dog\n"; }
        FreqDict d; d.loadFromFile(fname);
        assert(d.findWord("cat") == 2);
        assert(d.findWord("dog") == 3);
        std::remove(fname);

        const char* empty = "s4_empty.txt";
        { std::ofstream f(empty); }
        FreqDict d2; d2.loadFromFile(empty);
        assert(d2.size() == 0);
        std::remove(empty);

        bool caught = false;
        try { FreqDict d3; d3.loadFromFile("no_such_file_xyz.txt"); }
        catch (const std::runtime_error&) { caught = true; }
        assert(caught);
        std::cout << "7. loadFromFile           OK\n";
    }

    {
        FreqDict d;
        d.parseText("");
        assert(d.size() == 0);
        d.parseText("123 !@# 456");
        assert(d.size() == 0);
        std::cout << "8. parseText edge cases   OK\n";
    }

    {
        FreqDict d;
        for (int i = 0; i < 50; ++i)
            d.addWord("w" + std::to_string(i));
        d.addWord("w0"); d.addWord("w0");
        assert(d.size() == 50);
        auto all = d.getAllSorted();
        assert(all.size() == 50);
        assert(all[0].word_ == "w0" && all[0].count_ == 3);
        std::cout << "9. getAllSorted           OK\n";
    }

    {
        FreqDict d;
        d.parseText("hello hello world");
        DictStats s = d.getStats();
        assert(s.unique_ == 2);
        assert(s.total_  == 3);
        assert(s.average_ > 1.49 && s.average_ < 1.51);

        DictStats empty = FreqDict().getStats();
        assert(empty.unique_ == 0 && empty.total_ == 0 && empty.average_ == 0.0);
        std::cout << "10. stats                OK\n";
    }

    std::cout << "=== All tests passed ===\n\n";
}


int main(int argc, char* argv[])
{
#if defined(__unix__) || defined(__APPLE__)
    std::signal(SIGTSTP, handleSigtstp);
#endif

    bool runT = true;
    for (int i = 1; i < argc; ++i)
        if (std::string(argv[i]) == "--no-test") runT = false;
    if (runT) runTests();

    FreqDict dict;
    printHelp();
    std::cout << "> ";

    std::string line;
    while (std::getline(std::cin, line))
    {
        runCommand(dict, line);
        std::cout << "> ";
    }
    std::cout << "\nBye.\n";
    return 0;
}
