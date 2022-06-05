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
		void init(std::string& line);
		void ajouterVariable(std::string const& name, std::string const& value);
		bool isVariable(std::string& name);
		std::string getVariableValue(std::string const& name) const;
		std::unordered_map<std::string, std::string> getContent() const;

	private:
		std::unordered_map<std::string, std::string> m_variables;
};