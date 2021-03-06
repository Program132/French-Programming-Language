#pragma once

#include <string>

namespace FPL {
    enum BUILTIN_TYPE {
        VOID,
        INT,
        DOUBLE,
        STRING,
        AUTO
        // STRUCT -> pour l'oo
    };

    class Type {
    public:
        explicit Type(const std::string &name = "", const enum BUILTIN_TYPE type = VOID) : mName(name), mType(type){};

        std::string mName;
        enum BUILTIN_TYPE mType;
        // std::vector<Type> mField; -> pour le struct
    };
}