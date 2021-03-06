#include "Parser.h"

namespace FPL {
    Parser::Parser() {
        mTypes["vide"] = Type("vide", VOID);
        mTypes["entier"] = Type("entier", INT);
        mTypes["decimal"] = Type("decimal", DOUBLE);
        mTypes["texte"] = Type("texte", STRING);
        mTypes["auto"] = Type("auto", AUTO);
    }

    bool Parser::AppelerInstruction() {
        auto PossibleFonctionName = CheckerIdentifiant();
        if (PossibleFonctionName.has_value()) {
            if (isFonction(PossibleFonctionName->mText)) {
                FonctionDefinition fonction = mFonctions[PossibleFonctionName->mText];

                if (!fonction.HasArgument) {
                    if (!CheckerOperateur(";").has_value()) {
                        std::cerr << "Vous devez mettre le symbole ';' pour mettre fin a l'instruction." << std::endl;
                    }
                }

                if (fonction.HasArgument) {
                    if (CheckerOperateur(":").has_value()) {
                        while (fonction.NumberArgument > 0) {
                            auto name = CheckerIdentifiant();
                            if (!name.has_value()) {
                                std::cerr << "Veuillez specifier le nom du parametre." << std::endl;
                            }

                            if (!isFonctionArgument(PossibleFonctionName->mText, name->mText)) {
                                std::cerr << "Ce parametre n'existe pas dans la fonction " << PossibleFonctionName->mText << "." << std::endl;
                            }

                            auto value = CheckerValue();
                            if (!value.has_value()) {
                                std::cerr << "Veuillez donner une valeur au parametre '" << name->mText << "'." << std::endl;
                            }

                            auto parametre = getArgument(PossibleFonctionName->mText, name->mText);
                            if (parametre->ArgType.mType == value->StatementType.mType && parametre->ArgType.mType != AUTO) {
                                if (value->StatementType.mType == STRING) {
                                    std::replace(value->StatementName.begin(), value->StatementName.end(), '"', ' ');
                                }
                                parametre->ArgValue = value->StatementName;
                            } else if (parametre->ArgType.mType == AUTO && parametre->ArgType.mType != value->StatementType.mType) {
                                parametre->ArgType = Type("auto", AUTO);
                                parametre->ArgValue = value->StatementName;
                            } else {
                                std::cerr << "Le type de votre valeur doit ??tre identique a celui de l'argument." << std::endl;
                            }

                            ArgumentDefinition argument;
                            argument.ArgName = parametre->ArgName;
                            argument.ArgType = Type(parametre->ArgType.mName, parametre->ArgType.mType);
                            argument.ArgValue = parametre->ArgValue;
                            mArguments[fonction.FonctionName][argument.ArgName] = argument;

                            if (fonction.NumberArgument > 1 && !CheckerOperateur(",").has_value()) {
                                std::cerr << "Veuillez separer les different arguments par le symbole ','." << std::endl;
                            }
                            fonction.NumberArgument -= 1;
                        }
                        if (!CheckerOperateur(";").has_value()) {
                            std::cerr << "Vous devez mettre le symbole ';' pour mettre fin a l'instruction." << std::endl;
                        }
                    } else {
                        std::cerr << "La fonction a des parametres, vous devez obligatoirement leur donner une valeur." << std::endl;
                    }
                }

                if (!fonction.FonctionContent.empty()) {
                    std::string finalContent;
                    for (auto const &a : fonction.FonctionContent) {
                        finalContent.append(a).append(" ");
                    }
                    TokenBuilding t;
                    std::cout << "" << std::endl; // IGNORE (finalContent) -> sans le print, cela ne marche plus.
                    std::vector<Token> tokens = t.parseToken(finalContent);

                    auto FCurrToken = tokens.begin();
                    auto oldCurrentToken = mCurrentToken;
                    std::optional<FonctionDefinition> f = fonction;
                    parse(tokens, f);
                    mCurrentToken = oldCurrentToken;

                    std::vector<std::string> toErase;
                    for(auto const &var: mVariables) {
                        if(var.second.InFonction && !var.second.IsGlobal) {
                            toErase.push_back(var.first);
                        }
                    }
                    for(auto const &it: toErase)
                    {
                        mVariables.erase(it);
                    }
                }
                return true;
            }
        }
        return false;
    }

    bool Parser::FonctionInstruction(auto parseStart) {
        auto PeutEtreNom = CheckerIdentifiant();
        if (PeutEtreNom.has_value()) {
            if (CheckerOperateur("(").has_value()) {
                if (isFonction(PeutEtreNom->mText)) {
                    std::cerr << "Cette fonction est deja existante, donner un autre nom a cette derniere." << std::endl;
                    return false;
                }

                FonctionDefinition fonction;
                fonction.FonctionName = PeutEtreNom->mText;
                while (!CheckerOperateur(")").has_value()) {
                    auto type = CheckerType();

                    if (type->mType == VOID) { // Si aucun param??tre ou l'utilisateur a utilis?? l'argument 'vide' ou juste fermer.
                        if (CheckerOperateur(")").has_value()) {
                            break;
                        } else {
                            std::cerr << "Vous devez fermer les parentheses de la fonction." << std::endl;
                        }
                    }

                    if (!type.has_value()) {
                        std::cerr << "Vous devez sp??cifier un type d'argument." << std::endl;
                    }

                    // Ajout de l'argument...
                    auto possibleArg = CheckerIdentifiant();
                    if (!possibleArg.has_value()) {
                        std::cerr << "Vous devez sp??cifier un nom unique a l'argument." << std::endl;
                    }
                    ArgumentDefinition param;
                    param.ArgType.mName = type->mName;
                    param.ArgType.mType = type->mType;
                    param.ArgName = possibleArg->mText;
                    fonction.HasArgument = true;
                    fonction.NumberArgument += 1;
                    fonction.ArgsFonction.push_back(param);

                    if (CheckerOperateur(")").has_value()) {
                        break;
                    }
                    if (!CheckerOperateur(",").has_value()) {
                        std::cerr << "Vous devez utiliser la ',' pour separer les arguments de la fonction." << std::endl;
                    }
                }

                // On r??cup??re le code entre les {} :
                if (CheckerOperateur("{")) {
                    while (!CheckerOperateur("}").has_value()) {
                        if (mCurrentToken->mType == CHAINE_LITERAL) {
                            mCurrentToken->mText += "\"";
                        }
                        fonction.FonctionContent.push_back(mCurrentToken->mText);
                        ++mCurrentToken;

                        if (CheckerOperateur("}").has_value()) {
                            break;
                        }
                    }
                }

                mFonctions[fonction.FonctionName] = fonction;
                return true;
            } else {
                mCurrentToken = parseStart;
                std::cerr
                        << "Vous devez ouvrir les parenth??ses pour ajouter des param??tres si vous le souhaitez."
                        << std::endl;
            }
        } else {
            mCurrentToken = parseStart;
            std::cerr << "Vous devez donner un nom a votre fonction." << std::endl;
        }
        return false;
    }

    bool Parser::VariableInstruction(std::optional<FonctionDefinition>& fonction) {
        auto VarType = CheckerType();
        if (VarType.has_value()) {
            auto VarName = CheckerIdentifiant();

            if (isVariable(VarName->mText)) {
                std::cerr << "Veuillez choisir un autre nom pour votre variable." << std::endl;
                return false;
            }

            if (VarName.has_value()) {
                if (VarName->mText == "saisir") {
                    VarName = CheckerIdentifiant();
                    if (VarName.has_value()) {

                        if (fonction != std::nullopt) {
                            if (isArgument(fonction->FonctionName, VarName->mText)) {
                                std::cerr << "Le nom de la variable ne peut pas etre le meme que celui d'un argument de la fonction : " << fonction->FonctionName << "." << std::endl;
                                return false;
                            }
                        }

                        if (CheckerOperateur("-").has_value()) {
                            if (CheckerOperateur(">").has_value()) {
                                auto VarValue = CheckerValue();

                                if (VarValue->StatementType.mType == STRING) {
                                    std::replace(VarValue->StatementName.begin(), VarValue->StatementName.end(), '"', ' ');
                                }

                                if (VarValue.has_value()) {
                                    std::cout << VarValue->StatementName << std::endl;
                                } else {
                                    auto PossibleArgument = CheckerIdentifiant();
                                    if (PossibleArgument.has_value() && fonction != std::nullopt) {
                                        if (isArgument(fonction->FonctionName, PossibleArgument->mText)) {
                                            std::cout << mArguments[fonction->FonctionName][PossibleArgument->mText].ArgValue << std::endl;
                                        } else {
                                            std::cerr << "L'argument " << PossibleArgument->mText <<" est inexistant ." << std::endl;
                                        }
                                    }
                                }

                                if (CheckerOperateur(";").has_value()) {
                                    VariableDefinition variable;
                                    variable.VariableName = VarName->mText;
                                    variable.VariableType = Type(VarType->mName, VarType->mType);
                                    variable.IsGlobal = false;
                                    variable.InFonction = false;
                                    if (fonction.has_value() || fonction != std::nullopt) {
                                        variable.InFonction = true;
                                    }

                                    if (VarType->mType == INT) {
                                        int v;
                                        std::cin >> v;
                                        variable.VariableValue = std::to_string(v);
                                        mVariables[variable.VariableName] = variable;
                                    } else if (VarType->mType == DOUBLE) {
                                        double v;
                                        std::cin >> v;
                                        variable.VariableValue = std::to_string(v);
                                        mVariables[variable.VariableName] = variable;
                                    } else if (VarType->mType == STRING) {
                                        std::string v;
                                        std::cin >> v;
                                        variable.VariableValue = v;
                                        mVariables[variable.VariableName] = variable;
                                    }
                                    return true;
                                } else {
                                    std::cerr << "Merci de signifier la fin de la declaration de la variable avec ';'." << std::endl;
                                }
                            } else {
                                std::cerr << "Vous devez utiliser les symboles '->' pour donner une valeur." << std::endl;
                            }
                        } else {
                            if (CheckerOperateur(";").has_value()) {
                                VariableDefinition variable;
                                variable.VariableName = VarName->mText;
                                variable.VariableType = Type(VarType->mName, VarType->mType);
                                variable.IsGlobal = false;
                                variable.InFonction = false;
                                if (fonction.has_value() || fonction != std::nullopt) {
                                    variable.InFonction = true;
                                }

                                if (VarType->mType == INT) {
                                    int v;
                                    std::cin >> v;
                                    variable.VariableValue = std::to_string(v);

                                    mVariables[variable.VariableName] = variable;
                                } else if (VarType->mType == DOUBLE) {
                                    double v;
                                    std::cin >> v;
                                    variable.VariableValue = std::to_string(v);

                                    mVariables[variable.VariableName] = variable;
                                } else if (VarType->mType == STRING) {
                                    std::string v;
                                    std::cin >> v;
                                    variable.VariableValue = v;
                                    mVariables[variable.VariableName] = variable;
                                }
                                return true;
                            } else {
                                std::cerr << "Merci de signifier la fin de la declaration de la variable avec ';'." << std::endl;
                            }
                        }
                    } else {
                        std::cerr << "Vous devez indiquer un nom a la variable." << std::endl;
                    }
                }
                else if (VarName->mText == "globale") {
                    VarName = CheckerIdentifiant();
                    if (VarName.has_value()) {

                        if (fonction != std::nullopt) {
                            if (isArgument(fonction->FonctionName, VarName->mText)) {
                                std::cerr << "Le nom de la variable ne peut pas etre le meme que celui d'un argument de la fonction : " << fonction->FonctionName << "." << std::endl;
                                return false;
                            }
                        }

                        if (CheckerOperateur("-").has_value()) {
                            if (CheckerOperateur(">").has_value()) {
                                auto VarValue = CheckerValue();
                                if (VarValue.has_value()) {
                                    if (CheckerOperateur(";").has_value()) {
                                        VariableDefinition variable;
                                        variable.VariableName = VarName->mText;
                                        if (VarType->mType == AUTO) {
                                            variable.VariableType = Type(VarValue->StatementType.mName, VarValue->StatementType.mType);
                                        } else {
                                            if (VarType->mType != VarValue->StatementType.mType) {
                                                std::cerr << "Vous devez donner une valeur a la variable qui correspond au type." << std::endl;
                                                return false;
                                            }

                                            variable.VariableType = Type(VarType->mName, VarType->mType);
                                        }
                                        variable.IsGlobal = true;
                                        variable.InFonction = false;
                                        if (fonction.has_value() || fonction != std::nullopt) {
                                            variable.InFonction = true;
                                        }
                                        if (VarValue->StatementType.mType == STRING) {
                                            std::replace(VarValue->StatementName.begin(), VarValue->StatementName.end(), '"', ' ');
                                        }
                                        variable.VariableValue = VarValue->StatementName;

                                        mVariables[variable.VariableName] = variable;

                                        return true;
                                    }
                                } else {
                                    auto name = CheckerIdentifiant();
                                    if (name.has_value()) {
                                       if (CheckerOperateur(";").has_value()) {
                                           VariableDefinition variable;
                                           variable.VariableName = VarName->mText;
                                           variable.IsGlobal = true;
                                           variable.InFonction = false;
                                           if (VarType->mType != AUTO) {
                                               variable.VariableType = Type(VarType->mName, VarType->mType);
                                           }

                                           if (fonction.has_value() || fonction != std::nullopt) {
                                               variable.InFonction = true;
                                           }

                                           if (fonction != std::nullopt && isArgument(fonction->FonctionName, name->mText)) {
                                               variable.VariableValue = mArguments[fonction->FonctionName][name->mText].ArgValue;

                                               if (VarType->mType == AUTO) {
                                                   variable.VariableType = Type(mArguments[fonction->FonctionName][name->mText].ArgType.mName,
                                                                                mArguments[fonction->FonctionName][name->mText].ArgType.mType);
                                               } else if (VarType->mType != AUTO) {
                                                   if (VarType->mType != mArguments[fonction->FonctionName][name->mText].ArgType.mType) {
                                                       std::cerr << "Vous devez donner une valeur a la variable qui correspond au type." << std::endl;
                                                       return false;
                                                   }
                                               }
                                           } else if (isVariable(name->mText)) {
                                               variable.VariableValue = mVariables[name->mText].VariableValue;

                                               if (VarType->mType == AUTO) {
                                                   variable.VariableType = Type(mVariables[name->mText].VariableType.mName,
                                                                                mVariables[name->mText].VariableType.mType);
                                               } else if (VarType->mType != AUTO) {
                                                   if (VarType->mType != mVariables[name->mText].VariableType.mType) {
                                                       std::cerr << "Vous devez donner une valeur a la variable qui correspond au type." << std::endl;
                                                       return false;
                                                   }
                                               }
                                           }

                                           mVariables[variable.VariableName] = variable;
                                           return true;
                                       } else {
                                           std::cerr << "Merci de signifier la fin de la declaration de la variable avec ';'." << std::endl;
                                       }
                                    }
                                    std::cerr << "Vous devez donner une valeur a votre variable." << std::endl;
                                    return false;
                                }
                            } else {
                                std::cerr << "Vous devez utiliser les symboles '->' pour donner une valeur." << std::endl;
                            }
                        } else {
                            std::cerr << "Vous devez utiliser les symboles '->' pour donner une valeur." << std::endl;
                        }
                    } else {
                        std::cerr << "Vous devez indiquer un nom a la variable." << std::endl;
                    }
                }
                else {
                    if (fonction != std::nullopt && isArgument(fonction->FonctionName, VarName->mText)) {
                        std::cerr << "Le nom de la variable ne peut pas etre le meme que celui d'un argument de la fonction : " << fonction->FonctionName << "." << std::endl;
                        return false;
                    }

                    if (CheckerOperateur("-").has_value()) {
                        if (CheckerOperateur(">").has_value()) {
                            auto VarValue = CheckerValue();
                            if (VarValue.has_value()) {
                                if (CheckerOperateur(";").has_value()) {
                                    VariableDefinition variable;
                                    variable.VariableName = VarName->mText;
                                    if (VarType->mType == AUTO) {
                                        variable.VariableType = Type(VarValue->StatementType.mName, VarValue->StatementType.mType);
                                    } else {
                                        variable.VariableType = Type(VarType->mName, VarType->mType);
                                    }
                                    variable.IsGlobal = false;
                                    variable.InFonction = false;
                                    if (fonction.has_value() || fonction != std::nullopt) {
                                        variable.InFonction = true;
                                    }
                                    if (VarValue->StatementType.mType == STRING) {
                                        std::replace(VarValue->StatementName.begin(), VarValue->StatementName.end(), '"', ' ');
                                    }
                                    variable.VariableValue = VarValue->StatementName;

                                    mVariables[variable.VariableName] = variable;

                                    return true;
                                } else {
                                    std::cerr << "Merci de signifier la fin de la declaration de la variable avec ';'." << std::endl;
                                }
                            } else if (CheckerIdentifiant().has_value()) {
                                --mCurrentToken;
                                auto PossibleVariable = CheckerIdentifiant();
                                if (PossibleVariable.has_value()) {
                                    if (CheckerOperateur(";").has_value()) {
                                        if (isVariable(PossibleVariable->mText)) {
                                            auto OldVariable = mVariables[PossibleVariable->mText];
                                            if (OldVariable.VariableType.mType == VarType->mType) {
                                                VariableDefinition variable;
                                                variable.VariableName = VarName->mText;
                                                variable.VariableType = Type(VarType->mName, VarType->mType);
                                                variable.IsGlobal = false;
                                                variable.InFonction = false;
                                                if (fonction.has_value() || fonction != std::nullopt) {
                                                    variable.InFonction = true;
                                                }
                                                variable.VariableValue = OldVariable.VariableValue;

                                                mVariables[variable.VariableName] = variable;
                                                return true;
                                            } else {
                                                std::cerr << "Vous devez donner une valeur a la variable qui correspond au type." << std::endl;
                                                return false;
                                            }
                                        } else if (fonction != std::nullopt && isArgument(fonction->FonctionName, PossibleVariable->mText)) {
                                            VariableDefinition variable;
                                            variable.VariableName = VarName->mText;
                                            variable.IsGlobal = false;
                                            variable.InFonction = false;

                                            if (fonction.has_value() || fonction != std::nullopt) {
                                                variable.InFonction = true;
                                            }

                                            if (VarType->mType == mArguments[fonction->FonctionName][PossibleVariable->mText].ArgType.mType) {
                                                variable.VariableType = Type(VarType->mName, VarType->mType);
                                            } else {
                                                std::cerr << "Vous devez donner une valeur a la variable qui correspond au type." << std::endl;
                                                return false;
                                            }

                                            if (VarType->mType == AUTO) {
                                                variable.VariableType = Type(mArguments[fonction->FonctionName][PossibleVariable->mText].ArgType.mName,
                                                                             mArguments[fonction->FonctionName][PossibleVariable->mText].ArgType.mType);
                                            }

                                            variable.VariableValue = mArguments[fonction->FonctionName][PossibleVariable->mText].ArgValue;

                                            mVariables[variable.VariableName] = variable;
                                            return true;
                                        }
                                    } else {
                                        std::cerr << "Merci de signifier la fin de la d??claration de la variable avec ';'." << std::endl;
                                    }
                                }
                            } else {
                                std::cerr << "Vous devez donner une valeur a la variable qui correspond au type." << std::endl;
                            }
                        } else {
                            std::cerr << "Vous devez utiliser les symboles '->' pour donner une valeur a la variable." << std::endl;
                        }
                    } else {
                        std::cerr << "Vous devez utiliser les symboles '->' pour donner une valeur a la variable." << std::endl;
                    }
                }
            } else {
                std::cerr << "Vous devez indiquer un nom a la variable." << std::endl;
            }
        }
        return false;
    }

    bool Parser::ChangerInstruction(std::optional<FonctionDefinition>& fonction) {
        auto VarName = CheckerIdentifiant();
        if (VarName.has_value()) {
            if (isVariable(VarName->mText)) {
                auto VarType = mVariables[VarName->mText].VariableType;
                if (CheckerOperateur("-").has_value()) {
                    if (CheckerOperateur(">").has_value()) {
                        auto Value = CheckerValue();
                        if (Value.has_value()) {
                            if (Value->StatementType.mType == VarType.mType) {
                                if (CheckerOperateur(";").has_value()) {
                                    mVariables[VarName->mText].VariableValue = Value->StatementName;
                                    return true;
                                } else {
                                    std::cerr << "Vous devez mettre le symbole ';' pour mettre fin a l'instruction." << std::endl;
                                }
                            } else {
                                std::cerr << "Veuillez donner une valeur en rapport avec le type de la variable." << std::endl;
                            }
                        } else {
                            auto PossibleVar = CheckerIdentifiant();
                            if (PossibleVar.has_value()) {
                                if (CheckerOperateur(";").has_value()) {
                                    if (isVariable(PossibleVar->mText)) {
                                        if (mVariables[PossibleVar->mText].VariableType.mType == mVariables[VarName->mText].VariableType.mType) {
                                            mVariables[VarName->mText].VariableValue = mVariables[PossibleVar->mText].VariableValue;
                                            return true;
                                        } else {
                                            std::cerr << "Le type de la variable n'est pas le m??me que celui de la variable que vous voulez modifier." << std::endl;
                                        }
                                    } else if (fonction != std::nullopt) {
                                        if (isArgument(fonction->FonctionName, PossibleVar->mText)) {
                                            mVariables[VarName->mText].VariableValue = mArguments[fonction->FonctionName][PossibleVar->mText].ArgValue;
                                            return true;
                                        } else {
                                            std::cerr << "L'argument de la fonction ' " << fonction->FonctionName << " est inexistant." << std::endl;
                                        }
                                    } else {
                                        std::cerr << "La variable n'existe pas." << std::endl;
                                    }
                                } else {
                                    std::cerr << "Vous devez mettre le symbole ';' pour mettre fin a l'instruction." << std::endl;
                                }
                            }
                            std::cerr << "Veuillez preciser la nouvelle valeur de la variable." << std::endl;
                        }
                    } else {
                        std::cerr << "Vous devez utiliser les symboles '->' pour donner une valeur a la variable." << std::endl;
                    }
                } else {
                    std::cerr << "Vous devez utiliser les symboles '->' pour donner une valeur a la variable." << std::endl;
                }
            } else {
                std::cerr << "Cette variable n'existe pas." << std::endl;
            }
        } else {
            std::cerr << "Vous devez sp??cifier le nom de votre variable." << std::endl;
        }
        return false;
    }

    bool Parser::PrintInstruction(auto parseStart, std::optional<FonctionDefinition>& fonction) {
        auto Value = CheckerValue();
        if (Value.has_value()) {
            --mCurrentToken;
            while (!CheckerOperateur(";").has_value()) {
                Value = CheckerValue();
                if (Value.has_value()) {
                    if (Value->StatementType.mType == STRING) {
                        std::replace(Value->StatementName.begin(), Value->StatementName.end(), '"', ' ');
                    }

                    std::cout << Value->StatementName;
                    std::cout << " ";
                }
                else if (CheckerOperateur("[").has_value()) {
                    auto var = CheckerIdentifiant();
                    if (isVariable(var->mText)) {
                        if (CheckerOperateur("]").has_value()) {
                            auto finalMsg = mVariables[var->mText].VariableValue;
                            if (mVariables[var->mText].VariableType.mType == STRING) {
                                std::replace(finalMsg.begin(), finalMsg.end(), '"', ' ');
                            }
                            std::cout << finalMsg;
                        } else{
                            std::cerr << "Ces operateurs sont utilises dans cette instruction pour introduire une variable, merci de cloturer l'insertion avec ']'." << std::endl;
                            return false;
                        }
                    } else if (isArgument(fonction->FonctionName, var->mText)) {
                        if (CheckerOperateur("]").has_value()) {
                            auto finalMsg = mArguments[fonction->FonctionName][var->mText].ArgValue;
                            if (mArguments[fonction->FonctionName][var->mText].ArgType.mType == STRING) {
                                std::replace(finalMsg.begin(), finalMsg.end(), '"', ' ');
                            }
                            std::cout << finalMsg;
                        } else{
                            std::cerr << "Ces operateurs sont utilises dans cette instruction pour introduire une variable, merci de cloturer l'insertion avec ']'." << std::endl;
                            return false;
                        }
                    } else {
                        std::cerr << "La variable n'existe pas." << std::endl;
                    }
                } else {
                    std::cerr << "Vous devez mettre le symbole ';' pour mettre fin a l'instruction." << std::endl;
                    return false;
                }

                if (CheckerOperateur(";").has_value()) {
                    break;
                }
            }
            std::cout << std::endl;
            return true;
        } else {
            mCurrentToken = parseStart;
            ++mCurrentToken;
            auto value = CheckerIdentifiant();
            if (value.has_value()) {
                if (CheckerOperateur(";").has_value()) {
                    if (isVariable(value->mText)) {
                        std::cout << mVariables[value->mText].VariableValue << std::endl;
                        return true;
                    } else if (fonction != std::nullopt) {
                        if (isArgument(fonction->FonctionName, value->mText)) {
                            std::cout << mArguments[fonction->FonctionName][value->mText].ArgValue << std::endl;
                            return true;
                        }
                    }
                    std::cerr << "Vous devez specifier une argument d'une fonction ou une variable." << std::endl;
                    return false;
                } else {
                    std::cerr << "Vous devez mettre le symbole ';' pour mettre fin a l'instruction." << std::endl;
                }
            }
            std::cerr << "Vous devez ouvrir les guillemets pour transmettre une chaine de caracteres ou le nom de votre variable sous ce format : 'envoyer [variable];'." << std::endl;
        }
        return false;
    }

    bool Parser::ManagerInstruction(std::optional<FonctionDefinition>& fonction) {
        auto parseStart = mCurrentToken; // std::vector<Token>::iterator
        auto PeutEtreInstruction = CheckerIdentifiant();
        if (PeutEtreInstruction.has_value()) {
            if (PeutEtreInstruction->mText == "envoyer") {
                if (PrintInstruction(parseStart, fonction)) { return true; } else { return false; }
            } else if (PeutEtreInstruction->mText == "variable") {
                if (VariableInstruction(fonction)) { return true; } else { return false; }
            } else if (PeutEtreInstruction->mText == "changer") {
                if (ChangerInstruction(fonction)) { return true; } else { return false; }
            } else if (PeutEtreInstruction->mText == "definir") {
                if (FonctionInstruction(parseStart)) {return true;} else {return false;}
            } else if (PeutEtreInstruction->mText == "appeler") {
               if (AppelerInstruction()) { return true; } else {return false;}
            } else {
                mCurrentToken = parseStart;
            }
            return false;
        }
        return false;
    }



    void Parser::parse(std::vector<Token> &tokens, std::optional<FonctionDefinition>& fonction) {
        mEndToken = tokens.end();
        mCurrentToken = tokens.begin();

        while (mCurrentToken != mEndToken) { // Tant que tout le fichier n'est pas parcouru et qu'on n'a pas analys?? tous les ??l??ments.
            if (ManagerInstruction(fonction)) {

            } else {
                if (mCurrentToken->mText.empty() || mCurrentToken->mType == ESPACEVIDE ) {
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
        if (!name.empty() && mCurrentToken->mText != name) { return std::nullopt; }

        auto returnToken = mCurrentToken;
        ++mCurrentToken;
        return *returnToken;
    }

    std::optional<Token> Parser::CheckerOperateur(std::string_view name) {
        if (mCurrentToken == mEndToken) { return std::nullopt; }
        if (mCurrentToken->mType != OPERATEUR) { return std::nullopt; }
        if (mCurrentToken->mText != name && !name.empty()) { return std::nullopt; }
        // On v??rifie s'il est pr??sent, si c'est un OPERATEUR et que c'est le bon nom.

        auto returnToken = mCurrentToken; // std::vector<Token>::iterator
        ++mCurrentToken;
        return *returnToken; // On donne l'op??rateur
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

    bool Parser::isVariable(std::string &name) const {
        if (mVariables.contains(name)) { return true; }
        return false;
    }

    bool Parser::isFonction( std::string &name) const {
        if (mFonctions.contains(name)) { return true; }
        return false;
    }

    bool Parser::isArgument(std::string& fonction, std::string &name) const {
        const auto it = mArguments.find(fonction);
        if (it != mArguments.end())
        {
            return it->second.contains(name);
        }
        return false;
    }

    bool Parser::isFonctionArgument(std::string &fonction, std::string &argument) {
        if (isFonction(fonction)) {
            FonctionDefinition f = mFonctions[fonction];
            if (f.HasArgument) {
                for (auto const& a : f.ArgsFonction) {
                    if (a.ArgName == argument) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    std::optional<FonctionDefinition> Parser::getFonction(std::string& fonction) {
        if (isFonction(fonction)) {
            return mFonctions[fonction];
        }
        return std::nullopt;
    }

    std::optional<ArgumentDefinition> Parser::getArgument(std::string &fonction, std::string &name) {
        if (isFonctionArgument(fonction, name)) {
            auto f = getFonction(fonction);
            for (auto const &arg : f->ArgsFonction) {
                if (arg.ArgName == name) {
                    return arg;
                }
            }
        }
        return std::nullopt;
    }

    [[maybe_unused]] void Parser::DebugPrint() const {
        for (auto &funcPair: mFonctions) {
            for (auto &e: funcPair.second.FonctionContent) {
                std::cout << e << std::endl;
            }
        }
    }
}