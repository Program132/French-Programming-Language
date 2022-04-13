#include <iostream>
#include <string>

#include <fstream> // Pour le fichier de base: Programme.fpl

#include "headers/FPL_F.h" // Les functions de base

int main()
{
    std::ifstream file { "main.fpl" };

    std::string line;
    std::vector<std::string> content_lines_splitted;

    while (std::getline(file, line))
    {   
        if (line == " " || line == "") { continue; }

        std::string& ref_line = line;
        content_lines_splitted = split(ref_line);

        std::vector<std::string>& ref_content_line_splitted = content_lines_splitted;
        std::string& ref_line = line;

        FrenchProgrammingLanguage_Init(ref_content_line_splitted, ref_line); // On envoit tout et des références pour ne pas obtenir des copies
    } 

    std::cin.ignore();

    return 0;
}