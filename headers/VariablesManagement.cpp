#include "VariablesManagement.h"
#include "Types/MathType.h"

void VariablesManagement::ajouterVariable(std::string const& name, std::string const& value, std::string const& type) {
	m_variables.insert({ name, value });
	types_variables.insert({ name, type });
}

void VariablesManagement::setValueVariable(std::string const& name, std::string const& value) {
	m_variables[name] = value;
}

std::string VariablesManagement::getVariableValue(std::string const& name) const {
	std::unordered_map<std::string, std::string>::const_iterator got = m_variables.find(name);
	if (got != m_variables.end()) {
		return got->second;
	}
	else {
		return "FPL Erreur : Variable non trouve !";
	}
}

std::string VariablesManagement::getTypeVariable(std::string const& name) const {
	std::unordered_map<std::string, std::string>::const_iterator got = types_variables.find(name);
	if (got != types_variables.end()) {
		return got->second;
	}
	else {
		return "FPL Erreur : Variable non trouvé !";
	}
}

bool VariablesManagement::isVariable(std::string& name) {
	if (m_variables.find(name) == m_variables.end()) {
		return false;
	}
	else {
		return true;
	}
}

std::unordered_map<std::string, std::string> VariablesManagement::getContent() const {
	return m_variables;
}



void VariablesManagement::variables_basics(std::string& line) {
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
			std::string const type = "texte";
			var_type += " ";
			value_indication += " ";
			var_value = replaceFPL(line, var_name);
			var_value = replaceFPL(line, var_type);
			var_value = replaceFPL(line, value_indication);
			ajouterVariable(var_name, var_value, type);
		}
		else if (var_type == "math") {
			std::string const type = "math";
			var_type += " ";
			value_indication += " ";
			var_value = replaceFPL(line, var_name);
			var_value = replaceFPL(line, var_type);
			var_value = replaceFPL(line, value_indication);
			var_value = std::to_string(mathFinalValue(var_value));
			ajouterVariable(var_name, var_value, type);
		}

	}
	else {
		error = "basic";
		errorVariable(error);
	}
}

void VariablesManagement::variables_update(std::string& line) {
	std::string whatReplace = "changer ";
	std::string print_content = replaceFPL(line, whatReplace);

	std::vector<std::string> vec = split(print_content);

	std::string name, new_value, INDICATION, TYPE, error;
	bool normalContent = false;

	if (vec.size() == 0) {
		error = "name";
		errorUpdateVariable(error);
		normalContent = false;
	}
	else if (vec.size() == 1) {
		error = "indication";
		errorUpdateVariable(error);
		normalContent = false;
	}
	else if (vec.size() == 2) {
		error = "value";
		errorUpdateVariable(error);
		normalContent = false;
	}
	else {
		normalContent = true;
	}

	if (normalContent) {
		name = vec[0];
		INDICATION = vec[1];
		TYPE = getTypeVariable(name);
		new_value = replaceFPL(line, name);
		new_value = replaceFPL(line, INDICATION);

		if (TYPE == "math") {
			std::string const type = "math";
			new_value = std::to_string(mathFinalValue(new_value));
			setValueVariable(name, new_value);
		}
		else if (TYPE == "texte") {
			setValueVariable(name, new_value);
		}
	}
	else {
		error = "basic";
		errorUpdateVariable(error);
	}
}