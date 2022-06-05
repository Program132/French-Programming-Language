#pragma once

#include <iostream>
#include <vector>
#include <string> 
#include <sstream>
#include <stdexcept>
#include <algorithm>

std::vector<std::string> splittermathFinalValue(std::string& arg) {
    std::string myBuffer;

    std::vector<std::string> myVec;

    std::stringstream ss;
    ss << arg;
    while (ss >> myBuffer) {
        myVec.push_back(myBuffer);
    }

    return myVec;
}

int convertInt(std::string& n) {
    try {
        int v = std::stoi(n);
        return v;
    }
    catch (std::invalid_argument const& ex) {
        std::cout << "It can not be a int value !" << " Final error: " << ex.what() << std::endl;
    }

    return 0;
}

int mathFinalValue(std::string& content) {
    std::vector<std::string> vec = splittermathFinalValue(content);

    int i = 0;
    int finalValue = 0;

    for (auto const e : vec) {
        if (i == 0) {
            if (e == "+" || e == "-") {
                std::cout << "Do not start your calculation without a number ! " << std::endl;
                break;
            }
        }

        if (e == "+") {
            std::string numberBefore = vec[i - 1];
            std::string numberAfter = vec[i + 1];

            int numberB = convertInt(numberBefore);
            int numberA = convertInt(numberAfter);

            if (finalValue == 0) {
                finalValue += numberB + numberA;
            }
            else {
                finalValue += numberA;
            }
        }
        else if (e == "-") {
            std::string numberBefore = vec[i - 1];
            std::string numberAfter = vec[i + 1];

            int numberB = convertInt(numberBefore);
            int numberA = convertInt(numberAfter);

            if (finalValue == 0) {
                finalValue -= numberA - numberB;
            }
            else {
                finalValue -= numberA;
            }
        }
        else if (e == "*") {
            std::string numberBefore = vec[i - 1];
            std::string numberAfter = vec[i + 1];

            int numberB = convertInt(numberBefore);
            int numberA = convertInt(numberAfter);

            if (finalValue == 0) {
                finalValue = numberB * numberA;
            }
            else {
                finalValue *= numberA;
            }
        }
        else if (e == "/") {
            std::string numberBefore = vec[i - 1];
            std::string numberAfter = vec[i + 1];

            int numberB = convertInt(numberBefore);
            int numberA = convertInt(numberAfter);

            if (finalValue == 0) {
                finalValue = numberB / numberA;
            }
            else {
                finalValue /= numberA;
            }
        }
        i++;
    }

    return finalValue;
}