#include <iostream>
#include <string>
#include <iterator>

#include <fstream> // Pour le fichier de base: Programme.fpl

#include "headers/Utils.h" // Utilitaire
#include "headers/FPL_F.h" // Les functions de base

int main()
{
    std::ifstream file { "main.fpl" };

    std::string line;
    std::vector<std::string> content_lines_splitted;

    while (std::getline(file, line))
    {   
        if (line == " " || line == "") { continue; }

        content_lines_splitted = split(line);

        FrenchProgrammingLanguage_Init(content_lines_splitted, line);
    } 

    std::cin.ignore();

    return 0;
}