#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>


#include "Utils.h" // Utilitaires
#include "Management_Envoyer.h" // Pour executer: envoyer

// FPL

void FrenchProgrammingLanguage_Init(std::vector<std::string>& content_line_slitted, std::string& line) {
    if (content_line_slitted[0] == "envoyer") {
        FPL_Print_Init(content_line_slitted, line);
    }
    else {
        std::cout << "FPL Erreur : Je n'ai pas trouve votre demande !" << std::endl;
    }
}