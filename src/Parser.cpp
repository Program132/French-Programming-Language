#include "Parser.h"

namespace FPL {
    Parser::Parser() {
        mTypes["vide"] = Type("vide", VOID);
        mTypes["entier"] = Type("entier", INT);
        mTypes["decimal"] = Type("decimal", DOUBLE);
        mTypes["texte"] = Type("texte", STRING);
    }

    bool Parser::FunctionChecker(auto parseStart) {
        auto PeutEtreNom = CheckerIdentifiant();
        if (PeutEtreNom.has_value()) {
            // On a le nom d'une fonction.
            if (CheckerOperateur("(").has_value()) {
                //On a surement une fonction finie !

                FonctionDefinition fonction;
                fonction.FonctionName = PeutEtreNom->mText;

                while (CheckerOperateur(")") == std::nullopt) {
                    auto type = CheckerType();

                    // Si aucun paramètre ou l'utilisateur a utilisé l'argument 'vide' ou juste fermer.
                    if (type->mType == VOID) { break; }

                    if (!type.has_value() || type == std::nullopt) {
                        throw std::runtime_error("Vous devez spécifier un type d'argument.");
                    }

                    // Ajout de l'argument...
                    auto variableName = CheckerIdentifiant();
                    if (!variableName.has_value()) {
                        throw std::runtime_error("Vous devez spécifier un nom unique à l'argument.");
                    }
                    ArgumentDefinition param;
                    param.ArgType.mName = type->mName;
                    param.ArgType.mType = type->mType;
                    param.ArgName = variableName->mText;
                    fonction.ArgsFonction.push_back(param);

                    if (CheckerOperateur(")").has_value()) {
                        break;
                    }
                    if (!CheckerOperateur(",").has_value()) {
                        throw std::runtime_error(
                                "Vous devez utiliser la ',' pour separer les arguments de la fonction.");
                    }
                }

                mFonctions[fonction.FonctionName] = fonction;

                // On récupère le code entre les {} :

                if (CheckerOperateur("{")) {
                    while (!CheckerOperateur("}").has_value()) {
                        fonction.FonctionContent.push_back(mCurrentToken->mText);
                        ++mCurrentToken;

                        if (CheckerOperateur("}").has_value()) {
                            break;
                        }
                    }

                }

                return true;
            } else {
                mCurrentToken = parseStart;
                std::cerr
                        << "Vous devez ouvrir les parenthèses pour ajouter des paramètres si vous le souhaitez."
                        << std::endl;
            }
        } else {
            mCurrentToken = parseStart;
            std::cerr << "Vous devez donner un nom à votre fonction." << std::endl;
        }
        return false;
    }

    bool Parser::VariableInstruction(auto parseStart) {
        auto VarType = CheckerType();
        if (VarType.has_value()) {
            auto VarName = CheckerIdentifiant();

            if (isVariable(VarName->mText)) {
                std::cerr << "Veuillez choisir un autre nom pour votre variable." << std::endl;
                return false;
            }

            if (VarName.has_value()) {
                if (CheckerOperateur("-").has_value()) {
                    if (CheckerOperateur(">").has_value()) {
                        auto VarValue = CheckerValue();
                        if (VarValue.has_value()) {
                            if (VarValue->StatementType.mType == VarType->mType) {
                                if (CheckerOperateur("|")) {
                                    VariableDefinition variable;
                                    variable.VariableName = VarName->mText;
                                    variable.VariableType = Type(VarType->mName, VarType->mType);
                                    variable.VariableValue = VarValue->StatementName;

                                    mVariables[variable.VariableName] = variable;
                                    return true;
                                } else {
                                    std::cerr << "Merci de signifier la fin de la déclaration de la variable avec '|'." << std::endl;
                                }
                            } else {
                                std::cerr << "Vous devez donner une valeur qui est de même type que la variable." << std::endl;
                            }
                        } else {
                            std::cerr << "Vous devez donner une valeur a la variable qui correspond au type." << std::endl;
                        }
                    } else {
                        std::cerr << "Vous devez utiliser les symboles '->' pour donner une valeur à la variable." << std::endl;
                    }
                } else {
                    std::cerr << "Vous devez utiliser les symboles '->' pour donner une valeur à la variable." << std::endl;
                }
            } else {
                std::cerr << "Vous devez indiquer un nom à la variable." << std::endl;
            }
        }
        return false;
    }

    bool Parser::ChangerInstruction(auto parseStart) {
        auto VarName = CheckerIdentifiant();
        if (VarName.has_value()) {
            if (isVariable(VarName->mText)) {
                auto VarType = mVariables[VarName->mText].VariableType;
                if (CheckerOperateur("-").has_value()) {
                    if (CheckerOperateur(">").has_value()) {
                        auto Value = CheckerValue();
                        if (Value.has_value()) {
                            if (Value->StatementType.mType == VarType.mType) {
                                mVariables[VarName->mText].VariableValue = Value->StatementName;
                                return true;
                            } else {
                                std::cerr << "Veuillez donner une valeur en rapport avec le type de la variable." << std::endl;
                            }
                        } else {
                            std::cerr << "Veuillez preciser la nouvelle valeur de la variable." << std::endl;
                        }
                    } else {
                        std::cerr << "Vous devez utiliser les symboles '->' pour donner une valeur à la variable." << std::endl;
                    }
                } else {
                    std::cerr << "Vous devez utiliser les symboles '->' pour donner une valeur à la variable." << std::endl;
                }
            } else {
                std::cerr << "Cette variable n'existe pas." << std::endl;
            }
        } else {
            std::cerr << "Vous devez spécifier le nom de votre variable." << std::endl;
        }
        return false;
    }

    bool Parser::PrintInstruction(auto parseStart) {
        auto Value = CheckerValue();
        if (Value.has_value()) {
            if (Value->StatementType.mType == STRING) {
                std::replace(Value->StatementName.begin(), Value->StatementName.end(), '"', ' ');
            }
            std::cout << Value->StatementName << std::endl;
            return true;
        } else {
            mCurrentToken = parseStart;
            ++mCurrentToken;
            auto value = CheckerIdentifiant();
            if (value.has_value()) {
                if (CheckerOperateur("<").has_value()) {
                    if (CheckerOperateur("-").has_value()) {
                        if (isVariable(value->mText)) {
                            std::cout << mVariables[value->mText].VariableValue << std::endl;
                            return true;
                        } else {
                            mCurrentToken = parseStart;
                            std::cerr << "La variable n'existe pas." << std::endl;
                        }
                    }
                }
            }
            std::cerr << "Vous devez ouvrir les guillements pour transmettre une chaine de caractères ou le nom de votre variable sous ce format : 'envoyer (variable) <-" << std::endl;
        }
        return false;
    }

    bool Parser::ManagerInstruction() {
        auto parseStart = mCurrentToken; // std::vector<Token>::iterator
        auto PeutEtreInstruction = CheckerIdentifiant();
        if (PeutEtreInstruction.has_value()) {
            if (PeutEtreInstruction->mText == "envoyer") {
                if (PrintInstruction(parseStart)) { return true; } else { return false; }
            } else if (PeutEtreInstruction->mText == "variable") {
                if (VariableInstruction(parseStart)) { return true; } else { return false; }
            } else if (PeutEtreInstruction->mText == "changer") {
                if (ChangerInstruction(parseStart)) { return true; } else { return false; }
            } else if (PeutEtreInstruction->mText == "definir") {
                if (FunctionChecker(parseStart)) {return true;} else {return false;}
            } else{
                mCurrentToken = parseStart;
            }
            return false;
        }
        return false;
    }



    void Parser::parse(std::vector<Token> &tokens) {
        mEndToken = tokens.end();
        mCurrentToken = tokens.begin();

        while (mCurrentToken != mEndToken) { // Tant que tout le fichier n'est pas parcouru et qu'on n'a pas analysé tous les éléments.
            if (ManagerInstruction()) {

            } else {
                if (mCurrentToken->mText.empty()) {
                    continue;
                }

                std::cerr << "Identifier inconnu : " << mCurrentToken->mText << std::endl;
                ++mCurrentToken;
            }
        }
    }

    std::optional<Token> Parser::CheckerIdentifiant(std::string_view name) {
        if (mCurrentToken == mEndToken) { return std::nullopt; }
        if (mCurrentToken->mType != IDENTIFIANT) { return std::nullopt; }
        if (mCurrentToken->mText != name && !name.empty()) { return std::nullopt; }

        auto returnToken = mCurrentToken;
        ++mCurrentToken;
        return *returnToken;
    }

    std::optional<Token> Parser::CheckerOperateur(std::string_view name) {
        if (mCurrentToken == mEndToken) { return std::nullopt; }
        if (mCurrentToken->mType != OPERATEUR) { return std::nullopt; }
        if (mCurrentToken->mText != name && !name.empty()) { return std::nullopt; }
        // On vérifie s'il est présent, si c'est un OPERATEUR et que c'est le bon nom.

        auto returnToken = mCurrentToken; // std::vector<Token>::iterator
        ++mCurrentToken;
        return *returnToken; // On donne l'opérateur
    }

    std::optional<Type> Parser::CheckerType() {
        auto possibleType = CheckerIdentifiant();
        if (!possibleType.has_value()) { return std::nullopt; }

        auto foundType = mTypes.find(possibleType->mText);
        if (foundType == mTypes.end()) {
            --mCurrentToken;
            return std::nullopt;
        }

        return foundType->second;
    }

    std::optional<Statement> Parser::CheckerValue()
    {
        std::optional<Statement> res;
        if (mCurrentToken->mType == DECIMAL_LITERAL)
        {
            Statement doubleLitteralStatement (StatementKind::LITTERAL, mCurrentToken->mText, Type("decimal", DOUBLE));
            res = doubleLitteralStatement;
        }
        else if (mCurrentToken->mType == ENTIER_LITERAL)
        {
            Statement integerLitteralStatement(StatementKind::LITTERAL, mCurrentToken->mText, Type("entier", INT));
            res = integerLitteralStatement;
        }
        else if (mCurrentToken->mType == CHAINE_LITERAL)
        {
            Statement stringLitteralStatement(StatementKind::LITTERAL, mCurrentToken->mText, Type("text", STRING));
            res = stringLitteralStatement;

        } else {
            return std::nullopt;
        }

        ++mCurrentToken;
        return res;
    }


    void Parser::DebugPrint() const {
        for (auto &funcPair: mFonctions) {
            for (auto &e: funcPair.second.FonctionContent) {
                std::cout << e << std::endl;
            }
        }
    }

    bool Parser::isVariable(std::string &name) {
        if (mVariables.contains(name)) {
            return true;
        }
        return false;
    }
}