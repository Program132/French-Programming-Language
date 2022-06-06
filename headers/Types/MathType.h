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

double convertInt(std::string& n) {
    try {
        double v = std::stod(n);
        return v;
    }
    catch (std::invalid_argument const& ex) {
        std::cout << "It can not be a int value !" << " Final error: " << ex.what() << std::endl;
    }

    return 0;
}

double mathFinalValue(std::string& content) {
    std::vector<std::string> vec = splittermathFinalValue(content);

    int i = 0;
    double finalValue = 0;

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

            double numberB = convertInt(numberBefore);
            double numberA = convertInt(numberAfter);

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

            double numberB = convertInt(numberBefore);
            double numberA = convertInt(numberAfter);

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

            double numberB = convertInt(numberBefore);
            double numberA = convertInt(numberAfter);

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

            double numberB = convertInt(numberBefore);
            double numberA = convertInt(numberAfter);

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