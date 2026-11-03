#include "Parser.h"
#include <iostream>

using namespace std;

// =====================================================================
// Parser.cpp - implementacion del analizador sintactico (Entregable 1)
// ---------------------------------------------------------------------
// Idea central del descenso recursivo:
//   - Hay UNA funcion por cada no terminal de la gramatica.
//   - Cada funcion "consume" tokens hasta cerrar su produccion.
//   - La decision de que produccion aplicar se toma mirando currentToken
//     (lookahead de 1). Por eso la gramatica debe ser LL(1).
//
// Si viene algo que no esperabamos -> error() y seguimos. Asi podemos
// reportar varios errores en una sola corrida (recuperacion basica).
// =====================================================================

Parser::Parser(Lexer& lex) : lexer(lex), errorCount(0) {
    // Pedimos el primer token para que currentToken quede "cargado"
    // antes de empezar a parsear. Sin esto, la primera comparacion fallaria.
    currentToken = lexer.getNextToken();
}

// ---------- utilidades ----------

void Parser::advance() {
    currentToken = lexer.getNextToken();
}

bool Parser::match(TokenType type) {
    if (currentToken.type == type) {
        advance();
        return true;
    }
    return false;
}

void Parser::expect(TokenType type, const string& nombreEsperado) {
    if (currentToken.type == type) {
        advance();
    } else {
        error(nombreEsperado);
        // Nota: NO avanzamos aqui porque muchas veces el token actual
        // es util para la siguiente produccion (ej: si falta ';', el
        // token actual puede ser 'int' iniciando otra declaracion).
    }
}

void Parser::error(const string& esperado) {
    cerr << "[Error sintactico] Linea " << currentToken.line
         << ", Col " << currentToken.column
         << ": se esperaba " << esperado
         << " pero se encontro '" << currentToken.lexeme
         << "' (" << currentToken.getTypeString() << ")" << endl;
    errorCount++;
}

// ---------- punto de entrada ----------

bool Parser::parse() {
    parsePrograma();

    // Al terminar, lo unico que deberia quedar es TOKEN_EOF.
    // Si no, es que sobra codigo despues del } final.
    if (currentToken.type != TOKEN_EOF) {
        error("fin de archivo");
    }

    if (errorCount == 0) {
        cout << "\n[Parser] Programa ACEPTADO sintacticamente." << endl;
        return true;
    } else {
        cout << "\n[Parser] Programa RECHAZADO: "
             << errorCount << " error(es) sintactico(s)." << endl;
        return false;
    }
}

// ---------- no terminales ----------

// PROGRAMA -> LISTA_FUNCIONES PRINCIPAL
// En el Entregable 1 no reconocemos declaraciones de funciones del
// usuario (eso llega en el E2). Por eso saltamos directo a PRINCIPAL.
// La gramatica permite LISTA_FUNCIONES -> epsilon, asi que esto es valido.
void Parser::parsePrograma() {
    parsePrincipal();
}

// PRINCIPAL -> main ( ) BLOQUE
// Aclaracion: el PDF de especificaciones muestra el ejemplo "void main()".
// La gramatica formal solo pide "main ()", pero para que el ejemplo del
// PDF funcione, aceptamos un 'void' opcional antes de 'main'.
void Parser::parsePrincipal() {
    match(TOKEN_VOID);  // 'void' opcional (segun ejemplo del PDF)
    expect(TOKEN_MAIN, "'main'");
    expect(TOKEN_PAR_IZQ, "'('");
    expect(TOKEN_PAR_DER, "')'");
    parseBloque();
}

// BLOQUE -> { LISTA_VARIABLES COMANDOS }
// Importante: la gramatica obliga a que las DECLARACIONES vayan
// ANTES que los COMANDOS. No se puede declarar una variable en medio.
void Parser::parseBloque() {
    expect(TOKEN_LLA_IZQ, "'{'");
    parseListaVariables();
    parseComandos();
    expect(TOKEN_LLA_DER, "'}'");
}

// LISTA_VARIABLES -> TIPO id LISTA_ID ; LISTA_VARIABLES | epsilon
// Decidimos entre las dos alternativas mirando si currentToken es un TIPO_BASE.
// Si lo es, hay declaracion; si no, aplicamos epsilon (lista vacia).
void Parser::parseListaVariables() {
    while (currentToken.type == TOKEN_INT    ||
           currentToken.type == TOKEN_FLOAT  ||
           currentToken.type == TOKEN_CHAR   ||
           currentToken.type == TOKEN_BOOLEAN) {
        parseTipo();
        expect(TOKEN_ID, "identificador");
        parseListaId();
        expect(TOKEN_PYCOMA, "';' al final de la declaracion");
    }
    // Si no empieza con TIPO_BASE -> epsilon (no hacemos nada)
}

// LISTA_ID -> , id LISTA_ID | epsilon
// Permite declarar varias variables separadas por coma: int a, b, c;
void Parser::parseListaId() {
    while (match(TOKEN_COMA)) {
        expect(TOKEN_ID, "identificador despues de ','");
    }
}

// TIPO -> TIPO_BASE DIMENSION
void Parser::parseTipo() {
    parseTipoBase();
    parseDimension();
}

// TIPO_BASE -> char | float | int | boolean
void Parser::parseTipoBase() {
    if (match(TOKEN_INT))     return;
    if (match(TOKEN_FLOAT))   return;
    if (match(TOKEN_CHAR))    return;
    if (match(TOKEN_BOOLEAN)) return;
    error("tipo basico (int, float, char, boolean)");
}

// DIMENSION -> [ num_int ] DIMENSION | epsilon
// Soporta arreglos multidimensionales: int m[3][4];
void Parser::parseDimension() {
    while (match(TOKEN_COR_IZQ)) {
        expect(TOKEN_NUM_INT, "numero entero dentro de '[ ]'");
        expect(TOKEN_COR_DER, "']'");
    }
}

// COMANDOS -> COMANDO COMANDOS | epsilon
// Un comando siempre empieza con id (asignacion en E1). Mientras veamos
// un id, procesamos otro comando. Si viene '}' -> epsilon y terminamos.
void Parser::parseComandos() {
    while (currentToken.type == TOKEN_ID) {
        parseComando();
    }
    // En el Entregable 2 se agregan: if, while, scanf, println, return, func.
}

// COMANDO -> ASIGNACION (en E1; el resto llega en E2)
void Parser::parseComando() {
    parseAsignacion();
}

// ASIGNACION -> id = COMPLEMENTO ;
void Parser::parseAsignacion() {
    expect(TOKEN_ID, "identificador");
    expect(TOKEN_ASIGN, "'=' en la asignacion");
    parseComplemento();
    expect(TOKEN_PYCOMA, "';' al final de la asignacion");
}

// COMPLEMENTO -> EXPRESION | FUNCION
// En el E1 solo expresion simple. La llamada a funcion (func id(...))
// se implementa en el E2.
void Parser::parseComplemento() {
    parseExpresionSimple();
}

// Version minima de EXPRESION para el Entregable 1: solo un id o un numero.
// En el Entregable 2 se reemplaza por la jerarquia completa con precedencia
// (EXPRESION_O, EXPRESION_Y, EXPRESION_RELACIONAL, aditiva, multiplicativa, FACTOR).
void Parser::parseExpresionSimple() {
    if (match(TOKEN_ID))      return;
    if (match(TOKEN_NUM_INT)) return;
    if (match(TOKEN_NUM_DEC)) return;
    error("expresion (identificador o numero)");
    // Avanzamos para no quedarnos en bucle infinito con el mismo token malo.
    advance();
}
