#include "ErrorsFPL.h"

void errorInstruction() {
	std::cout << "FPL Erreur : Je n'ai pas trouve votre demande !" << std::endl;
}

void errorVariable(std::string& name) {
	if (name == "name") {
		std::cout << "FPL Erreur : Vous devez indiquer le nom de votre variable !" << std::endl;
	}
	else if (name == "type") {
		std::cout << "FPL Erreur : Vous devez indiquer le type de votre variable !" << std::endl;
	}
	else if (name == "indication") {
		std::cout << "FPL Erreur : Vous devez indiquer le symbole -> pour dire au programme que vous attribuez une valeur a votre variable !" << std::endl;
	}
	else if (name == "value") {
		std::cout << "FPL Erreur : Vous devez donner une valeur a votre variable !" << std::endl;
	}
	else if (name == "wrong type") {
		std::cout << "FPL Erreur : Le type de votre variable n'est pas valide ! " << std::endl;
	}
	else if (name == "basic") {
		std::cout << "FPL Erreur : Votre variable ne peut pas etre declare !" << std::endl;
	}
}