#include "FonctionDefinition.h"

namespace FPL {
    void FonctionDefinition::FDebugPrint() const {
        std::cout << FonctionName << "(" << std::endl;
        for (ArgumentDefinition arg : ArgsFonction) {
            arg.ADebugPrint();
        }
        std::cout << ")\n" << std::endl;
    }

    void ArgumentDefinition::ADebugPrint() const {
        std::cout << ArgName << "->" << ArgType.mName << ", " << std::endl;
    }
}