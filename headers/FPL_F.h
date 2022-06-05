#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <unordered_map>
#include <iterator>


#include "Utils/Utils.h" // Utilitaires
#include "Utils/ErrorsFPL.h" // Management Erreurs

#include "Management_Envoyer.h" // Pour l'instruction: envoyer
#include "VariablesManagement.h" // Pour init les variables

// FPL

void FrenchProgrammingLanguage_Init(VariablesManagement& var, std::vector<std::string>& content_line_slitted, std::string& line);