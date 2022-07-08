#include "VariableDefinition.h"

namespace FPL {
    Statement::Statement(StatementKind kind, std::string name, Type type) {
        StatementType = type;
        StatementName = name;
        StatementKind{kind};
    }

    void VariableDefinition::GiveValue(const std::string &value) {
        std::cout << VariableName << " de type " << VariableType.mName << " aura la valeur " << value;

        if (VariableType.mType == ENTIER_LITERAL) {

        } else if (VariableType.mType == DECIMAL_LITERAL) {

        } else if (VariableType.mType == CHAINE_LITERAL) {

        } else {

        }
    }
}
