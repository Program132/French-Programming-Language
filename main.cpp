#include "src/TokenBuilding.h"
#include "src/Parser.h"
#include <iostream>
#include <fstream>

int main() {
    std::cout << "F.P.L. : Chargement\n\n" << std::endl;

    std::ifstream file {"../main.fpl"};
    if (!file) { std::cerr << "Fichier introuvable." << std::endl; return 1; }

    std::string f_content((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));

    //std::cout << f_content << std::endl;

    FPL::TokenBuilding t;
    std::vector<FPL::Token> tokens = t.parseToken(f_content);

    /*
        for (auto &token : tokens) {
            token.DebugPrint();
        }
     */

    FPL::Parser parser;
    parser.parse(tokens);

    parser.DebugPrint();

    return 0;
}
