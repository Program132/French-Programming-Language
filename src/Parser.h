#pragma once

#include <iostream>
#include <vector>
#include <string>
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
        std::optional<Type> CheckerType(const std::string &name = std::string());
        std::optional<Token> CheckerIdentifiant(const std::string &name = std::string());
        std::optional<Token> CheckerOperateur(const std::string &name = std::string());
        std::optional<Token> CheckerChaineLitteral(const std::string &name = std::string());
        std::optional<Statement> CheckerValue();


        // Print :
        bool PrintStat();

        // Variable :
        bool VariableDeclaration();
        std::map<std::string, VariableDefinition> mVariables;

        // Fonctions :
        bool FunctionChecker();
        std::map<std::string, FonctionDefinition> mFonctions;


        // Utils :
        std::vector<Token>::iterator mCurrentToken;
        std::vector<Token>::iterator mEndToken;
        std::map<std::string, Type> mTypes;
    };
}