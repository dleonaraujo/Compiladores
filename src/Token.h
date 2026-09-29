#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum TokenType {
    // Números
    TOKEN_NUM_INT,
    TOKEN_NUM_DEC,

    // Identificadores y texto
    TOKEN_ID,
    TOKEN_TEXTO,

    // Palabras reservadas
    TOKEN_INT,
    TOKEN_FLOAT,
    TOKEN_CHAR,
    TOKEN_BOOLEAN,
    TOKEN_VOID,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_FOR,
    TOKEN_WHILE,
    TOKEN_SCANF,
    TOKEN_PRINTLN,
    TOKEN_MAIN,
    TOKEN_RETURN,

    // Operador de asignacion
    TOKEN_ASIGN,

    // Operadores aritmeticos
    TOKEN_MAS,
    TOKEN_MENOS,
    TOKEN_POR,
    TOKEN_DIV,
    TOKEN_MOD,

    // Operadores logicos
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_NOT,

    // Operador de comparacion / relacional
    TOKEN_COMP,

    // Simbolos especiales
    TOKEN_PAR_IZQ,
    TOKEN_PAR_DER,
    TOKEN_COR_IZQ,
    TOKEN_COR_DER,
    TOKEN_LLA_IZQ,
    TOKEN_LLA_DER,
    TOKEN_COMA,
    TOKEN_PYCOMA,

    // Auxiliares
    TOKEN_DESCONOCIDO,
    TOKEN_EOF
};

struct Token {
    TokenType type;
    std::string lexeme;
    int line;
    int column;
    int attribute = -1;

    std::string getTypeString() const {
        switch (type) {
            case TOKEN_NUM_INT:    return "NUM_INT";
            case TOKEN_NUM_DEC:    return "NUM_DEC";
            case TOKEN_ID:         return "ID";
            case TOKEN_TEXTO:      return "TEXTO";
            case TOKEN_INT:        return "INT";
            case TOKEN_FLOAT:      return "FLOAT";
            case TOKEN_CHAR:       return "CHAR";
            case TOKEN_BOOLEAN:    return "BOOLEAN";
            case TOKEN_VOID:       return "VOID";
            case TOKEN_IF:         return "IF";
            case TOKEN_ELSE:       return "ELSE";
            case TOKEN_FOR:        return "FOR";
            case TOKEN_WHILE:      return "WHILE";
            case TOKEN_SCANF:      return "SCANF";
            case TOKEN_PRINTLN:    return "PRINTLN";
            case TOKEN_MAIN:       return "MAIN";
            case TOKEN_RETURN:     return "RETURN";
            case TOKEN_ASIGN:      return "ASIGN";
            case TOKEN_MAS:        return "MAS";
            case TOKEN_MENOS:      return "MENOS";
            case TOKEN_POR:        return "POR";
            case TOKEN_DIV:        return "DIV";
            case TOKEN_MOD:        return "MOD";
            case TOKEN_AND:        return "AND";
            case TOKEN_OR:         return "OR";
            case TOKEN_NOT:        return "NOT";
            case TOKEN_COMP:       return "COMP";
            case TOKEN_PAR_IZQ:    return "PAR_IZQ";
            case TOKEN_PAR_DER:    return "PAR_DER";
            case TOKEN_COR_IZQ:    return "COR_IZQ";
            case TOKEN_COR_DER:    return "COR_DER";
            case TOKEN_LLA_IZQ:    return "LLA_IZQ";
            case TOKEN_LLA_DER:    return "LLA_DER";
            case TOKEN_COMA:       return "COMA";
            case TOKEN_PYCOMA:     return "PYCOMA";
            case TOKEN_DESCONOCIDO:return "error    ";
            case TOKEN_EOF:        return "EOF";
            default:               return "UNKNOWN";
        }
    }
};

#endif
