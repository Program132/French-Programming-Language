#include "Management_Envoyer.h"

void FPL_Print_Init(VariablesManagement& var, std::string& line) {
    // Préparation
    std::string whatReplace = "envoyer ";
    std::string print_content = replaceFPL(line, whatReplace);
    std::vector<std::string> vec = split(print_content);

    bool continuer = true;
    if (vec.size() == 1) {
        if (var.isVariable(vec[0])) {
            std::cout << var.getVariableValue(vec[0]) << std::endl;
        }
        continuer = false;
    }

    if (continuer) {
        int i = 0;
        for (auto const e : vec) {
            if (e == "/n") {
                vec[i] = "";
                std::cout << "\n";
                continue;
            }
            std::cout << e << " ";
            i++;
        }
        std::cout << "\n";
    }
}