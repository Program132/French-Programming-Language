#include "VariableDefinition.h"

namespace FPL {
    Statement::Statement(StatementKind kind, std::string name, Type type) {
        StatementType = type;
        StatementName = name;
        StatementKind{kind};
    }
}
