#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <string>
#include <vector>

class SymbolTable {
private:
    std::vector<std::string> entries;

public:
    int insert(const std::string& lexeme);
    void print() const;
    int size() const;
};

#endif
