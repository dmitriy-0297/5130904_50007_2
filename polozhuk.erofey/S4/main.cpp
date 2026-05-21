#include <iostream>
#include <exception>
#include "crossref.hpp"
#include "runme.hpp"


int main() {
    try {
        polozhuk::run_me();
    }
    catch (...) {
        std::cerr << "The program is terminating forcefully." << std::endl;
        return 1;
    }

    return 0;
}