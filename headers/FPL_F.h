#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include "Management_Envoyer.h"

// Utils

std::vector<std::string> split(std::string& arg) {
    std::string myBuffer;

    std::vector<std::string> myVec;

    std::stringstream ss;
    ss << arg;
    while (ss >> myBuffer) {
        myVec.push_back(myBuffer);
    }

    return myVec;
}

std::vector<char> splitChar(std::string& arg) {
    std::istringstream iss(arg);
    std::vector<char> vec((std::istream_iterator<char>(iss)), std::istream_iterator<char>());;

    return vec;
}

std::string replace(std::string& line, std::string& what) {
    std::string result = line.replace(line.find(what), what.length(), "");
    return result;
}



// FPL

void FrenchProgrammingLanguage_Init(std::vector<std::string>& content_line_slitted, std::string& line) {
    if (content_line_slitted[0] == "envoyer") {
        FPL_Print_Init(content_line_slitted, line);
    }
    else {
        std::cout << "FPL Erreur : Je n'ai pas trouve votre demande !" << std::endl;
    }
}