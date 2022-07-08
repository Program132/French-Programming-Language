#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "TypeManager.h"

namespace FPL {
    class ArgumentDefinition {
    public:
        std::string ArgName;
        Type ArgType;

        void ADebugPrint() const;
    };

    class FonctionDefinition {
    public:
        std::string FonctionName;
        std::vector<ArgumentDefinition> ArgsFonction;
        std::vector<std::string> FonctionContent;

        void FDebugPrint() const;
    };
}