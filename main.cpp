#include "src/TokenBuilding.h"
#include "src/Parser.h"
#include <iostream>
#include <string>
#include <fstream>

int main() {
    //std::cout << "F.P.L. : Chargement\n\n" << std::endl;

    std::string fileName;
    std::cout << "Fichier : ";
    std::cin >> fileName;
    std::cout << std::endl;

    std::ifstream file { fileName};
    std::ifstream file2 {"../" + fileName};
    if (!file && !file2) { std::cerr << "Fichier introuvable." << std::endl; return 1; }

    if (!file2) {
        std::string f_content((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
        FPL::TokenBuilding t;
        std::vector<FPL::Token> tokens = t.parseToken(f_content);
        FPL::Parser parser;
        parser.parse(tokens);
        parser.DebugPrint();
    } else if (!file) {
        std::string f_content((std::istreambuf_iterator<char>(file2)), (std::istreambuf_iterator<char>()));
        FPL::TokenBuilding t;
        std::vector<FPL::Token> tokens = t.parseToken(f_content);
        FPL::Parser parser;
        parser.parse(tokens);
        parser.DebugPrint();
    }

    char c;
    std::cin >> c;
    return 0;
}
