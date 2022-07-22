#pragma once

#include <iostream>
#include <string>
#include <map>

#include "TokenBuilding.h"
#include "TypeManager.h"

namespace FPL {

    enum class StatementKind: int {
        VARIABLE_DECLARATION,
        FUNCTION_CALL,
        LITTERAL
    };

    static const char* sStatementKindStrings[] = {
            "VARIABLE_DECLARATION",
            "FUNCTION_CALL",
            "LITTERAL"
    };

    class Statement {
    public:
        Statement();
        Statement(StatementKind kind, std::string name, Type type);

        std::string StatementName;
        Type StatementType{Type("void", VOID)};
        StatementKind kind{};
    };

    class VariableDefinition {
    public:
        std::string VariableName;
        Type VariableType;
        std::string VariableValue;
        bool InFonction;
        bool IsGlobal;
    };
}