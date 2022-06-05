#include "VariablesManagement.h"
#include "Types/MathType.h"

void VariablesManagement::ajouterVariable(std::string const& name, std::string const& value) {
	m_variables.insert({ name, value });
}

std::string VariablesManagement::getVariableValue(std::string const& name) const {
	std::unordered_map<std::string, std::string>::const_iterator got = m_variables.find(name);
	if (got != m_variables.end()) {
		return got->second;
	}
	else {
		return "FPL Erreur : Variable non trouvé !";
	}
}

bool VariablesManagement::isVariable(std::string& name) {
	std::unordered_map<std::string, std::string>::const_iterator got = m_variables.find(name);

	if (got != m_variables.end()) {
		return true;
	}
	else {
		return false;
	}
}

std::unordered_map<std::string, std::string> VariablesManagement::getContent() const {
	return m_variables;
}



void VariablesManagement::init(std::string& line) {
	std::string whatReplace = "variable ";
	std::string print_content = replaceFPL(line, whatReplace);

	std::vector<std::string> vec = split(print_content);

	std::string var_type, var_name, value_indication, var_value, error;

	bool normalContent = false;

	if (vec.size() == 0) {
		error = "type";
		errorVariable(error);
		normalContent = false;
	}
	else if (vec.size() == 1) {
		error = "name";
		errorVariable(error);
		normalContent = false;
	}
	else if (vec.size() == 2) {
		error = "indication";
		errorVariable(error);
		normalContent = false;
	}
	else if (vec.size() == 3) {
		error = "value";
		errorVariable(error);
		normalContent = false;
	}
	else {
		normalContent = true;
	}

	if (normalContent) {
		var_type = vec[0];
		var_name = vec[1];
		value_indication = vec[2];

		if (var_type == "texte") {
			var_type += " ";
			value_indication += " ";

			var_value = replaceFPL(line, var_name);
			var_value = replaceFPL(line, var_type);
			var_value = replaceFPL(line, value_indication);

			ajouterVariable(var_name, var_value);
		}
		else if (var_type == "math") {
			var_type += " ";
			value_indication += " ";

			var_value = replaceFPL(line, var_name);
			var_value = replaceFPL(line, var_type);
			var_value = replaceFPL(line, value_indication);

			var_value = std::to_string(mathFinalValue(var_value));

			ajouterVariable(var_name, var_value);
		}
		
	}
	else {
		error = "basic";
		errorVariable(error);
	}
}