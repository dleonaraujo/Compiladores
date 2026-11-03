#ifndef PARSER_H
#define PARSER_H

#include "Lexer.h"
#include "Token.h"
#include <string>

// =====================================================================
// Analizador Sintactico para LP (Entregable 1 - 30%)
// ---------------------------------------------------------------------
// Tecnica: descenso recursivo (exigido por el PDF de especificaciones).
// Cada funcion parseXxx() implementa UN no terminal de la gramatica LP.
// El parser NO lee caracteres: consume tokens pidiendoselos al Lexer
// uno por uno con getNextToken(). El contrato con el Lexer esta en
// Lexer.h: el parser solo mira 'currentToken' y lo avanza.
//
// Alcance de esta fase (segun seccion 4.1 del PDF):
//   PROGRAMA, PRINCIPAL, BLOQUE, LISTA_VARIABLES, LISTA_ID,
//   TIPO, TIPO_BASE, DIMENSION, COMANDO -> ASIGNACION, COMPLEMENTO
//   + deteccion de errores basicos (;, parentesis, llaves, token inesperado).
// El resto de la gramatica (expresiones completas, if/while, scanf/println,
// funciones, return) se agregara en el Entregable 2.
// =====================================================================
class Parser {
private:
    Lexer& lexer;         // Se guarda por referencia: el parser USA al lexer, no lo posee.
    Token currentToken;   // Token "bajo el cursor". Es la unica ventana del parser al codigo fuente.
    int errorCount;       // Contador para reportar al final cuantos errores sintacticos hubo.

    // ----- Utilidades del parser -----
    // Pide el siguiente token al lexer y lo guarda en currentToken.
    // Es el equivalente del advance() del lexer, pero a nivel de tokens.
    void advance();

    // Si el token actual es del tipo esperado, lo consume y devuelve true.
    // Si no coincide, NO consume nada y devuelve false. Util para producciones
    // opcionales (ej: DIMENSION -> [ num_int ] DIMENSION | epsilon).
    bool match(TokenType type);

    // Como match, pero si no coincide reporta error. Se usa cuando la gramatica
    // obliga a un token concreto (ej: tras 'main' DEBE venir '(').
    void expect(TokenType type, const std::string& nombreEsperado);

    // Imprime un mensaje de error con linea, columna, lexema encontrado
    // y lo esperado. Incrementa errorCount. No lanza excepcion para poder
    // seguir analizando (recuperacion basica).
    void error(const std::string& esperado);

    // ----- Funciones de los no terminales (una por produccion) -----
    // Entregable 1: solo las necesarias para declaraciones y asignaciones.
    void parsePrograma();         // PROGRAMA -> LISTA_FUNCIONES PRINCIPAL  (en E1 solo PRINCIPAL)
    void parsePrincipal();        // PRINCIPAL -> main ( ) BLOQUE
    void parseBloque();           // BLOQUE -> { LISTA_VARIABLES COMANDOS }
    void parseListaVariables();   // LISTA_VARIABLES -> TIPO id LISTA_ID ; LISTA_VARIABLES | epsilon
    void parseListaId();          // LISTA_ID -> , id LISTA_ID | epsilon
    void parseTipo();             // TIPO -> TIPO_BASE DIMENSION
    void parseTipoBase();         // TIPO_BASE -> char | float | int | boolean
    void parseDimension();        // DIMENSION -> [ num_int ] DIMENSION | epsilon
    void parseComandos();         // COMANDOS -> COMANDO COMANDOS | epsilon
    void parseComando();          // COMANDO -> (en E1 solo ASIGNACION; se amplia en E2)
    void parseAsignacion();       // ASIGNACION -> id = COMPLEMENTO ;
    void parseComplemento();      // COMPLEMENTO -> EXPRESION | FUNCION (en E1 solo expresion simple)

    // Version simplificada de EXPRESION para el Entregable 1:
    // acepta un unico id / num_int / num_dec. La jerarquia completa con
    // precedencia (||, &&, comp, +-, */ %, factor) llega en el Entregable 2.
    void parseExpresionSimple();

public:
    // Se construye con el lexer ya cargado (loadFile hecho desde main).
    explicit Parser(Lexer& lex);

    // Punto de entrada. Devuelve true si el programa es sintacticamente
    // correcto (errorCount == 0 al terminar). Imprime "aceptado" o el
    // numero de errores encontrados.
    bool parse();
};

#endif
