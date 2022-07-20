#include "src/TokenBuilding.h"
#include "src/Parser.h"
#include <iostream>
#include <string>
#include <fstream>

int main() {
    std::string fileName;
    std::cout << "Fichier : ";
    std::cin >> fileName;
    std::cout << std::endl;

    std::ifstream file { fileName};
    std::string f_content((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
    FPL::TokenBuilding t;
    std::vector<FPL::Token> tokens = t.parseToken(f_content);
    FPL::Parser parser;
    parser.parse(tokens);
    //parser.DebugPrint();
    return 0;
}
