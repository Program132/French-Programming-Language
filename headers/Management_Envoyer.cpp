#include "Management_Envoyer.h"
#include "Utils/ErrorsFPL.h"

void FPL_Print_Init(VariablesManagement& var, std::string& line) {
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
        for (auto e : vec) {
            std::vector<char> tab_char_element = splitChar(e);

            if (tab_char_element[0] == '[' && tab_char_element.back() == ']') {
                std::vector<std::string> elements_delete = { "[", "]" };
                std::string variable_element = replaceFPL(e, elements_delete[0]);
                variable_element = replaceFPL(e, elements_delete[1]);

                std::string variable_element_final = var.getVariableValue(variable_element);
                std::cout << variable_element_final << " ";
                continue;
            }
            else if (tab_char_element[0] == '[' && tab_char_element.back() != ']') {
                std::cout << "\n";
                errorIntroduireVariable();
                break;
            }

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