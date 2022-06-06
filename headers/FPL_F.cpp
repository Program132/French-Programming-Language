#include "FPL_F.h"

void FrenchProgrammingLanguage_Init(VariablesManagement& var, std::vector<std::string>& content_line_slitted, std::string& line) {
    if (content_line_slitted[0] == "envoyer") {
        FPL_Print_Init(var, line);
    }
    else if (content_line_slitted[0] == "variable") {
        var.init(line);
    }
    else {
        errorInstruction();
    }
}