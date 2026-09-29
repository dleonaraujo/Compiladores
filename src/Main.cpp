#include "Lexer.h"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    Lexer lexer;

    if (argc >= 2) {
        if (!lexer.loadFile(argv[1])) {
            return 1;
        }
    } else {
        string input((istreambuf_iterator<char>(cin)),
                      istreambuf_iterator<char>());
        lexer.setSource(input);
    }

    cout << "===== LISTA DE TOKENS =====" << endl;
    cout << "TIPO\t\tLEXEMA\t\tLINEA\tCOL\tTOKEN" << endl;
    cout << "---------------------------------------------------------" << endl;

    Token token = lexer.getNextToken();
    while (token.type != TOKEN_EOF) {
        cout << token.getTypeString() << "\t\t"
             << token.lexeme << "\t\t"
             << token.line << "\t"
             << token.column << "\t";

        if (token.type == TOKEN_ID) {
            cout << "<ID," << token.attribute << ">";
        } else {
            cout << "<" << token.getTypeString() << ">";
        }
        cout << endl;

        token = lexer.getNextToken();
    }

    cout << "EOF\t\t\t\t" << token.line << "\t" << token.column << endl;

    lexer.getSymbolTable().print();

    return 0;
}
