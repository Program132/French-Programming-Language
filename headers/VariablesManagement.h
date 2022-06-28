#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>

#include "Utils/Utils.h" // Utilitaires
#include "Utils/ErrorsFPL.h"

class VariablesManagement
{
public:
	void variables_basics(std::string& line);
	void variables_update(std::string& line);

	void ajouterVariable(std::string const& name, std::string const& value, std::string const& type);
	bool isVariable(std::string& name);
	std::string getVariableValue(std::string const& name) const;
	std::string getTypeVariable(std::string const& name) const;
	void setValueVariable(std::string const& name, std::string const& value);
	std::unordered_map<std::string, std::string> getContent() const;

private:
	std::unordered_map<std::string, std::string> m_variables;
	std::unordered_map<std::string, std::string> types_variables;
};