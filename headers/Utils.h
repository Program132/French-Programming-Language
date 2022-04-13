#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>

std::vector<std::string> split(std::string& arg);

std::vector<char> splitChar(std::string& arg);

std::string replaceFPL(std::string& line, std::string& what);