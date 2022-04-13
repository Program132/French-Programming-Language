#pragma once

#include <iostream>
#include <string>
#include <iterator>

#include "Utils.h" // Utilitaires


void FPL_Print_Init(std::vector<std::string>& content_line_slitted, std::string& line) {
	// Préparation

	std::string whatReplace = "envoyer ";
	std::string print_content = replaceFPL(line, whatReplace);


	// Envoyer 

	std::cout << print_content << std::endl;
}