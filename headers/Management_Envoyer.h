#pragma once

#include <iostream>
#include <string>


void FPL_Print_Init(std::vector<std::string>& content_line_slitted, std::string& line) {
	// Préparation

	std::string whatReplace = "envoyer ";
	std::string& ref_whatreplace = whatReplace;
	std::string print_content = replace(line, ref_whatreplace);


	// Envoyer 

	std::cout << print_content << std::endl;
}