#include <iostream>
#include <exception>
#include "crossref.hpp"
#include "runme.hpp"


int main() {
    try {
        polozhuk::run_me();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    catch (...) {
        std::cerr << "Error" << std::endl;
        return 1;
    }

    return 0;
}