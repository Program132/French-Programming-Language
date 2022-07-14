#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include <stdexcept>
#include <optional>
#include <map>
#include <algorithm>

#include "TokenBuilding.h"
#include "TypeManager.h"
#include "FonctionDefinition.h"
#include "VariableDefinition.h"

namespace FPL {

    class Parser {
    public:
        Parser();

        void parse(std::vector<Token> &tokens);

        void DebugPrint() const;

    private:
        // Essentielles:
        std::optional<Type> CheckerType(std::string_view name = std::string());
        std::optional<Token> CheckerIdentifiant(std::string_view name = std::string());
        std::optional<Token> CheckerOperateur(std::string_view name = std::string());
        std::optional<Statement> CheckerValue();


        // Print : In the ManagerInstruction

        // Variable :
        std::map<std::string, VariableDefinition> mVariables;
        bool isVariable(std::string &name);

        // Fonctions :
        bool FunctionChecker();
        std::map<std::string, FonctionDefinition> mFonctions;


        // Utils :
        std::vector<Token>::iterator mCurrentToken;
        std::vector<Token>::iterator mEndToken;
        std::map<std::string, Type> mTypes;

        bool ManagerInstruction();


    };
}