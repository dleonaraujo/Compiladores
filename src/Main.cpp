#include "Lexer.h"
#include "Parser.h"
#include <iostream>
#include <string>

using namespace std;

// =====================================================================
// Verificado por Diego Leon
// Main: punto de entrada del compilador LP.
// Flujo (segun seccion 3.1 del PDF):
//   archivo.lp -> Lexer -> tokens -> Parser -> aceptado / errores
//
// Uso:
//   Main.exe archivo.lp            -> solo analisis sintactico
//   Main.exe archivo.lp --tokens   -> imprime tokens y luego analiza
// =====================================================================
int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " <archivo.lp> [--tokens]" << endl;
        return 1;
    }

    // La bandera --tokens es para depurar o para la evidencia del lexer
    // (Entregable anterior). Por defecto NO se imprimen los tokens porque
    // el foco de este proyecto es el analizador sintactico.
    bool showTokens = (argc >= 3 && string(argv[2]) == "--tokens");

    // ---------- Fase opcional: imprimir lista de tokens ----------
    // Usamos un lexer SEPARADO porque imprimir consume todos los tokens.
    // Si usaramos el mismo lexer para el parser, ya no quedarian tokens.
    if (showTokens) {
        Lexer lexerLex;
        if (!lexerLex.loadFile(argv[1])) return 1;

        cout << "===== LISTA DE TOKENS =====" << endl;
        cout << "TIPO\t\tLEXEMA\t\tLINEA\tCOL\tTOKEN" << endl;
        cout << "---------------------------------------------------------" << endl;

        Token token = lexerLex.getNextToken();
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
            token = lexerLex.getNextToken();
        }
        cout << "EOF\t\t\t\t" << token.line << "\t" << token.column << endl;
    }

    // ---------- Fase principal: analisis sintactico ----------
    Lexer lexer;
    if (!lexer.loadFile(argv[1])) return 1;

    cout << "\n===== ANALISIS SINTACTICO =====" << endl;
    Parser parser(lexer);
    bool ok = parser.parse();

    // La tabla de simbolos se imprime SIEMPRE porque es evidencia pedida
    // desde el entregable del lexer y sigue siendo util para revisar que
    // todos los identificadores se registraron.
    lexer.getSymbolTable().print();

    // Codigo de salida: 0 = aceptado, 1 = errores (util si lo llamas desde scripts)
    return ok ? 0 : 1;
}
