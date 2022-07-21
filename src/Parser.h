#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include <stdexcept>
#include <optional>
#include <map>
#include <functional>
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

        [[maybe_unused]] void DebugPrint() const;

    private:
        // Essentielles:
        std::optional<Type> CheckerType();
        std::optional<Token> CheckerIdentifiant(std::string_view name = std::string());
        std::optional<Token> CheckerOperateur(std::string_view name = std::string());
        std::optional<Statement> CheckerValue();


        // Print :
        bool PrintInstruction(auto parseStart);

        // Variable :
        std::map<std::string, VariableDefinition> mVariables;
        bool isVariable(std::string &name) const;
        bool VariableInstruction();
        bool ChangerInstruction();

        // Fonctions :
        std::map<std::string, FonctionDefinition> mFonctions;
        bool FonctionInstruction(auto parseStart);
        bool AppelerInstruction();
        bool isFonction(std::string &name) const;
        bool isFonctionArgument(std::string &fonction, std::string &argument);
        std::optional<FonctionDefinition> getFonction(std::string &fonction);
        std::optional<ArgumentDefinition> getArgument(std::string &fonction, std::string &name);


        // Utils :
        bool ManagerInstruction();
        std::vector<Token>::iterator mCurrentToken;
        std::vector<Token>::iterator mEndToken;
        std::map<std::string, Type> mTypes;
    };
}