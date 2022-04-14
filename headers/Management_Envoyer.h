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
    std::vector<std::string> vec = split(print_content);

    int i = 0;
    for (auto const e : vec) {
        if (e == "/n") {
            vec[i] = "";
            std::cout << "\n";
            continue;
        }
        std::cout << e << " ";
        i++;
    }
    std::cout << "\n";

	//std::cout << print_content << std::endl;
}