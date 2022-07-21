#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "TypeManager.h"
#include "VariableDefinition.h"

namespace FPL {
    class ArgumentDefinition {
    public:
        std::string ArgName;
        Type ArgType;
        std::string ArgValue;

        void ADebugPrint() const;
    };

    class FonctionDefinition {
    public:
        std::string FonctionName;
        std::vector<ArgumentDefinition> ArgsFonction;
        std::vector<std::string> FonctionContent;
        bool HasArgument = false;
        int NumberArgument = 0;

        [[maybe_unused]] void FDebugPrint() const;
    };
}