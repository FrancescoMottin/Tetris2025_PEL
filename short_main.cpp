#include <sstream>
#include <iostream>
#include "tetris.cpp"

int main() {
    std::stringstream ss("2 123 ( [] () () [] )");
    piece p1;
    ss >> p1;

    if (ss.fail()) {
        std::cerr << "Parsing fallito!" << std::endl;
        return 1;
    }

    std::cout << "Output serializzato:\n" << p1 << std::endl;
    return 0;
}
