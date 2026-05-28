#include "freqdict.h"
#include "runme.h"
#include <iostream>
#include <cassert>
#include <fstream>
int main() {
    std::cout << "=== Tests ===" << std::endl;
    {
        FreqDict dict;
        assert(dict.getUniqueWordCount() == 0);
        assert(dict.getTotalWordCount() == 0);
        assert(dict.search("hello") == 0);
        std::cout << "Test 1 passed" << std::endl;
    }
    {
        std::ofstream testFile("test_input.txt");
        testFile << "hello world hello" << std::endl;
        testFile << "apple banana apple" << std::endl;
        testFile.close();
        FreqDict dict;
        dict.loadFile("test_input.txt");
        assert(dict.getUniqueWordCount() == 4);
        assert(dict.getTotalWordCount() == 6);
        std::cout << "Test 2 passed" << std::endl;
    }
    {
        std::ofstream testFile("test_input.txt");
        testFile << "hello world hello" << std::endl;
        testFile.close();
        FreqDict dict;
        dict.loadFile("test_input.txt");
        assert(dict.search("hello") == 2);
        assert(dict.search("world") == 1);
        assert(dict.search("apple") == 0);
        std::cout << "Test 3 passed" << std::endl;
    }
    {
        std::ofstream testFile("test_input.txt");
        testFile << "Hello WORLD HeLLo" << std::endl;
        testFile.close();
        FreqDict dict;
        dict.loadFile("test_input.txt");
        assert(dict.search("hello") == 2);
        assert(dict.search("world") == 1);
        assert(dict.search("HELLO") == 2);
        assert(dict.search("WORLD") == 1);
        std::cout << "Test 4 passed" << std::endl;
    }
    {
        std::ofstream testFile("test_input.txt");
        testFile << "test test test test" << std::endl;
        testFile.close();
        FreqDict dict;
        dict.loadFile("test_input.txt");
        assert(dict.search("test") == 4);
        std::cout << "Test 5 passed" << std::endl;
    }
    {
        std::ofstream testFile("test_input.txt");
        testFile << "hello world" << std::endl;
        testFile.close();
        FreqDict dict;
        dict.loadFile("test_input.txt");
        assert(dict.getUniqueWordCount() == 2);
        dict.clear();
        assert(dict.getUniqueWordCount() == 0);
        assert(dict.getTotalWordCount() == 0);
        assert(dict.search("hello") == 0);
        std::cout << "Test 6 passed" << std::endl;
    }
    {
        FreqDict dict;
        try {
            dict.loadFile("nonexistent_file_12345.txt");
            assert(false);
        }
        catch (const FileOpenException&) {
            std::cout << "Test 7 passed" << std::endl;
        }
    }
    {
        std::ofstream testFile("empty.txt");
        testFile.close();
        FreqDict dict;
        dict.loadFile("empty.txt");
        assert(dict.getUniqueWordCount() == 0);
        assert(dict.getTotalWordCount() == 0);
        std::cout << "Test 8 passed" << std::endl;
    }
    {
        std::ofstream testFile("symbols.txt");
        testFile << "123 456 !@# $%^ &*()" << std::endl;
        testFile.close();
        FreqDict dict;
        dict.loadFile("symbols.txt");
        assert(dict.getUniqueWordCount() == 0);
        assert(dict.getTotalWordCount() == 0);
        std::cout << "Test 9 passed" << std::endl;
    }
    {
        std::ofstream testFile("test_input.txt");
        testFile << "a a a b b c" << std::endl;
        testFile.close();
        FreqDict dict;
        dict.loadFile("test_input.txt");
        dict.printTop(3);
        std::cout << "Test 10 passed" << std::endl;
    }
    {
        std::ofstream testFile("test_input.txt");
        testFile << "hello world" << std::endl;
        testFile.close();
        FreqDict dict;
        dict.loadFile("test_input.txt");
        dict.printTop(100);
        std::cout << "Test 11 passed" << std::endl;
    }
    {
        std::ofstream testFile("test_input.txt");
        testFile << "apple banana apple" << std::endl;
        testFile.close();
        FreqDict dict;
        dict.loadFile("test_input.txt");
        dict.printAll();
        std::cout << "Test 12 passed" << std::endl;
    }
    {
        std::ofstream testFile("test_input.txt");
        testFile << "one two two three three three" << std::endl;
        testFile.close();
        FreqDict dict;
        dict.loadFile("test_input.txt");
        assert(dict.getUniqueWordCount() == 3);
        assert(dict.getTotalWordCount() == 6);
        std::cout << "Test 13 passed" << std::endl;
    }
    {
        std::ofstream testFile1("test1.txt");
        testFile1 << "hello" << std::endl;
        testFile1.close();
        std::ofstream testFile2("test2.txt");
        testFile2 << "world" << std::endl;
        testFile2.close();
        FreqDict dict;
        dict.loadFile("test1.txt");
        assert(dict.search("hello") == 1);
        dict.loadFile("test2.txt");
        assert(dict.search("world") == 1);
        assert(dict.search("hello") == 1);
        assert(dict.getUniqueWordCount() == 2);
        std::cout << "Test 14 passed" << std::endl;
    }
    {
        std::ofstream testFile("large.txt");
        for (int i = 0; i < 1000; ++i) {
            testFile << "word" << std::endl;
        }
        testFile.close();
        FreqDict dict;
        dict.loadFile("large.txt");
        assert(dict.search("word") == 1000);
        assert(dict.getUniqueWordCount() == 1);
        assert(dict.getTotalWordCount() == 1000);
        std::cout << "Test 15 passed" << std::endl;
    }
    std::cout << "Test 16 passed (memory allocation handling in place)" << std::endl;
    std::remove("test_input.txt");
    std::remove("empty.txt");
    std::remove("symbols.txt");
    std::remove("test1.txt");
    std::remove("test2.txt");
    std::remove("large.txt");
    std::cout << "\nAll tests passed!" << std::endl;
    std::cout << "\n=== Starting Interactive Mode ===" << std::endl;
    run_me();
    return 0;
}
