#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

namespace FPL {
    enum TokenTypes {
        ESPACEVIDE,
        IDENTIFIANT,
        COMMENTAIRE,
        PEUTETRE_COMMENTAIRE,
        ENTIER_LITERAL, // Ints
        CHAINE_LITERAL, // Strings
        OPERATEUR, // Opérateur comme '+', '-', '*', '/', '(', '{', '[', ',',
        CHAINE_ESPACE_SEQUENCE,
        PEUTETRE_DOUBLE,
        DECIMAL_LITERAL
    };

    static const char *sTokenTypes[] = {
            "ESPACEVIDE",
            "IDENTIFIANT",
            "COMMENTAIRE",
            "PEUTETRE_COMMENTAIRE",
            "ENTIER_LITERAL",
            "CHAINE_LITERAL",
            "OPERATEUR",
            "CHAINE_ESPACE_SEQUENCE",
            "PEUTETRE_DOUBLE",
            "DECIMAL_LITERAL"
    };

    class Token {
    public:
        enum TokenTypes mType; // Type du token
        std::string mText; // Contenue du token
        size_t mLineNumber{1}; // Ligne de code

        [[maybe_unused]] void DebugPrint() const;
    };

    class TokenBuilding {
    public:
        std::vector<Token> parseToken(const std::string &content);


    private:
        static void endToken(Token &token, std::vector<Token> &tokens);
    };
}
