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
        Statement(StatementKind kind, std::string name, Type type);

        std::string StatementName;
        Type StatementType{Type("void", VOID)};
        StatementKind kind{};
    };

    class VariableDefinition {
    public:
        //VariableDefinition(std::string name, std::string typeName, enum BUILTIN_TYPE type) : VariableName(name), VariableType(Type(typeName, type)){};

        std::string VariableName;
        Type VariableType;

        int valueVarI; // Valeur si c'est un int
        double valueVarD; // Valeur si c'est un décimal
        std::string valueVarS; // Valeur si c'est une chaine de caractere (en FPL, texte)

        void GiveValue(const std::string &value);
    };
}