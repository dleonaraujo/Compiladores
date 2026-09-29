#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include "Token.h"
#include "SymbolTable.h"

class Lexer {
private:
    std::string source;
    size_t cursor;
    int currentLine;
    int currentColumn;
    SymbolTable symbols;
    std::unordered_map<std::string, TokenType> keywords;

    void initKeywords();
    char peek() const;
    char advance();

public:
    Lexer();
    bool loadFile(const std::string& filepath);
    void setSource(const std::string& input);
    Token getNextToken();
    const SymbolTable& getSymbolTable() const;
};

#endif
