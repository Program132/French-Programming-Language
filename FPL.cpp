#include <iostream>
#include <string>
#include <iterator>

#include <fstream> // Pour le fichier de base

#include "headers/Utils/Utils.h" // Utilitaire
#include "headers/FPL_F.h" // Les functions de base
#include "headers/VariablesManagement.h"

int main()
{
    std::ifstream file { "main.fpl" };

    std::string line;
    std::vector<std::string> content_lines_splitted;

    VariablesManagement variables;

    while (std::getline(file, line))
    {   
        if (line == " " || line == "") { continue; }

        content_lines_splitted = split(line);

        FrenchProgrammingLanguage_Init(variables, content_lines_splitted, line);
    } 

    std::cin.ignore();

    return 0;
}