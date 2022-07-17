#include "TokenBuilding.h"

namespace FPL {
    std::vector<Token> TokenBuilding::parseToken(const std::string &content) {
        std::vector<Token> tokens;
        Token currentToken;

        for (auto &currT : content) // char
        {
            if (currentToken.mType == CHAINE_ESPACE_SEQUENCE) {
                switch(currT) {
                    case 'n':
                        currentToken.mText.append(1, '\n');
                        break;
                    case 'r':
                        currentToken.mText.append(1, '\r');
                        break;
                    case 't':
                        currentToken.mText.append(1, '\t');
                        break;
                    case '\\':
                        currentToken.mText.append(1, '\\');
                        break;
                    default:
                        throw std::runtime_error(std::string("Inconnu : \\") + std::string(1, currT) +
                                                 " dans la ligne " + std::to_string(currentToken.mLineNumber) + ".");
                }

                currentToken.mType = CHAINE_LITERAL;
                continue;
            }
            else if (currentToken.mType == PEUTETRE_COMMENTAIRE && currT != '/') {
                currentToken.mType = OPERATEUR;
                endToken(currentToken, tokens);
                continue;
            }

            switch(currT)
            {
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                    if (currentToken.mType == ESPACEVIDE) {
                        currentToken.mType = ENTIER_LITERAL;
                        currentToken.mText.append(1, currT);
                    } else if (currentToken.mType == PEUTETRE_DOUBLE) {
                        currentToken.mType = DECIMAL_LITERAL;
                        currentToken.mText.append(1, currT);
                    } else {
                        currentToken.mText.append(1, currT);
                    }
                    break;


                case '.':
                    if (currentToken.mType == ESPACEVIDE) {
                        currentToken.mType = PEUTETRE_DOUBLE;
                        currentToken.mText.append(1, currT);
                    } else if (currentToken.mType == ENTIER_LITERAL) {
                        currentToken.mType = DECIMAL_LITERAL;
                        currentToken.mText.append(1, currT);
                    } else if (currentToken.mType == CHAINE_LITERAL) {
                        currentToken.mText.append(1, currT);
                    } else {
                        endToken( currentToken, tokens);
                        currentToken.mType = OPERATEUR;
                        currentToken.mText.append(1, currT);
                        endToken(currentToken, tokens);
                    }
                    break;


                case '{':
                case '}':
                case '(':
                case ')':
                case '=':
                case '+':
                case '-':
                case '*':
                case '>':
                case '<':
                case ';':
                case ',':
                case '|':
                    if (currentToken.mType != CHAINE_LITERAL) {
                        endToken(currentToken, tokens);
                        currentToken.mType = OPERATEUR;
                        currentToken.mText.append(1, currT);
                        endToken(currentToken, tokens);
                    } else {
                        currentToken.mText.append(1, currT);
                    }
                    break;


                case '\t':
                case ' ':
                    if (currentToken.mType == CHAINE_LITERAL || currentToken.mType == COMMENTAIRE)
                    {
                        currentToken.mText.append(1, currT);
                    }
                    else {
                        endToken(currentToken, tokens);
                    }
                    break;


                case '\r':
                case '\n':
                    endToken(currentToken, tokens);
                    ++currentToken.mLineNumber;
                    break;


                case '"':
                    if (currentToken.mType != CHAINE_LITERAL) {
                        endToken(currentToken, tokens);
                        currentToken.mType = CHAINE_LITERAL;
                        currentToken.mText.append(1, currT);
                    } else if (currentToken.mType == CHAINE_LITERAL)  {
                        endToken(currentToken, tokens);
                    }
                    break;


                case '\\':
                    if (currentToken.mType == CHAINE_LITERAL) {
                        endToken(currentToken, tokens);
                        currentToken.mType = CHAINE_LITERAL;
                        currentToken.mText.append(1, currT);
                    } else  {
                        endToken(currentToken, tokens);
                        currentToken.mType = OPERATEUR;
                        currentToken.mText.append(1, currT);
                        endToken(currentToken, tokens);
                    }
                    break;

                case '/':
                    if (currentToken.mType == CHAINE_LITERAL) {
                        currentToken.mText.append(1, currT);
                    } else if (currentToken.mType == PEUTETRE_COMMENTAIRE) {
                        currentToken.mType = COMMENTAIRE;
                        currentToken.mText.erase();
                    } else {
                        endToken(currentToken, tokens);
                        currentToken.mType = PEUTETRE_COMMENTAIRE;
                        currentToken.mText.append(1, currT);
                    }
                    break;

                default:
                    if (currentToken.mType == ESPACEVIDE || currentToken.mType == ENTIER_LITERAL
                        || currentToken.mType == DECIMAL_LITERAL)
                    {
                        endToken(currentToken, tokens);
                        currentToken.mType = IDENTIFIANT;
                        currentToken.mText.append(1, currT);
                    }
                    else {
                        currentToken.mText.append(1, currT);
                    }
                    break;
            }
        }

        return tokens;
    }

    void TokenBuilding::endToken(Token &token, std::vector<Token> &tokens) {
        if (token.mType == COMMENTAIRE) {
            //std::cout << "Commentaire ignoree : '" << token.mText << "'." << std::endl;
        } else if (token.mType != ESPACEVIDE) {
            tokens.push_back(token);
        }
        if (token.mType == PEUTETRE_DOUBLE)
        {
            if (token.mText == ".") {
                token.mType = OPERATEUR;
            }
            else {
                token.mType = DECIMAL_LITERAL;
            }
        }
        token.mType = ESPACEVIDE;
        token.mText.erase();
    }

    [[maybe_unused]] void Token::DebugPrint() const {
        std::cout << "Token(" << sTokenTypes[mType] << ", '" << mText << "'." << std::endl;
    }
}