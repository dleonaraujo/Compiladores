#include "SymbolTable.h"
#include <iostream>

using namespace std;

int SymbolTable::insert(const string& lexeme) {
    for (size_t i = 0; i < entries.size(); ++i) {
        if (entries[i] == lexeme) return static_cast<int>(i);
    }
    entries.push_back(lexeme);
    return static_cast<int>(entries.size() - 1);
}

void SymbolTable::print() const {
    cout << "\n===== TABLA DE SIMBOLOS =====" << endl;
    cout << "Posicion\tIdentificador" << endl;
    cout << "-----------------------------" << endl;
    for (size_t i = 0; i < entries.size(); ++i) {
        cout << i << "\t\t" << entries[i] << endl;
    }
}

int SymbolTable::size() const {
    return static_cast<int>(entries.size());
}
