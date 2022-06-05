#include "Utils.h"

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

std::string replaceFPL(std::string& line, std::string& what) {
    std::string result = line.replace(line.find(what), what.length(), "");
    return result;
}