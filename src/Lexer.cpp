#include "Lexer.h"

using namespace std;

// Funciones auxiliares propias (sin usar <cctype>)
// D = [0-9]
static bool esDigito(char c) {
    return c >= '0' && c <= '9';
}

// L = [a-zA-Z_]
static bool esLetra(char c) {
    if (c >= 'a' && c <= 'z') return true;
    if (c >= 'A' && c <= 'Z') return true;
    if (c == '_') return true;
    return false;
}

// L | D
static bool esLetraODigito(char c) {
    return esLetra(c) || esDigito(c);
}

Lexer::Lexer() : source(""), cursor(0), currentLine(1), currentColumn(1) {
    initKeywords();
}

void Lexer::initKeywords() {
    keywords["int"]     = TOKEN_INT;
    keywords["float"]   = TOKEN_FLOAT;
    keywords["char"]    = TOKEN_CHAR;
    keywords["boolean"] = TOKEN_BOOLEAN;
    keywords["void"]    = TOKEN_VOID;
    keywords["if"]      = TOKEN_IF;
    keywords["else"]    = TOKEN_ELSE;
    keywords["for"]     = TOKEN_FOR;
    keywords["while"]   = TOKEN_WHILE;
    keywords["scanf"]   = TOKEN_SCANF;
    keywords["println"] = TOKEN_PRINTLN;
    keywords["main"]    = TOKEN_MAIN;
    keywords["return"]  = TOKEN_RETURN;
}

bool Lexer::loadFile(const string& filepath) {
    ifstream file(filepath);
    if (!file.is_open()) {
        cerr << "Error al abrir el archivo: " << filepath << endl;
        return false;
    }
    string content((istreambuf_iterator<char>(file)),
                    istreambuf_iterator<char>());
    source = content;
    cursor = 0;
    currentLine = 1;
    currentColumn = 1;
    file.close();
    return true;
}

void Lexer::setSource(const string& input) {
    source = input;
    cursor = 0;
    currentLine = 1;
    currentColumn = 1;
}

char Lexer::peek() const {
    if (cursor >= source.length()) return '\0';
    return source[cursor];
}

char Lexer::advance() {
    if (cursor >= source.length()) return '\0';
    char c = source[cursor++];
    currentColumn++;
    return c;
}

const SymbolTable& Lexer::getSymbolTable() const {
    return symbols;
}

Token Lexer::getNextToken() {
    while (peek() != '\0') {
        char c = peek();

        // Saltos de linea y espacios en blanco
        if (c == '\n') {
            currentLine++;
            currentColumn = 1;
            cursor++;
            continue;
        }
        if (c == ' ' || c == '\t' || c == '\r') {
            advance();
            continue;
        }

        int startColumn = currentColumn;

        // COMENTARIO de linea:  //.*\n  (se ignora)
        if (c == '/' && (cursor + 1 < source.length()) && source[cursor + 1] == '/') {
            while (peek() != '\n' && peek() != '\0') {
                advance();
            }
            continue; // no se emite token para el comentario
        }

        // NUM_INT (D+) y NUM_DEC (D+.D+)
        if (esDigito(c)) {
            size_t start = cursor;
            while (esDigito(peek())) advance();

            bool isDecimal = false;
            if (peek() == '.' && (cursor + 1 < source.length())
                && esDigito(source[cursor + 1])) {
                isDecimal = true;
                advance(); // consumir el punto
                while (esDigito(peek())) advance();
            }

            string lexeme = source.substr(start, cursor - start);
            return {isDecimal ? TOKEN_NUM_DEC : TOKEN_NUM_INT,
                    lexeme, currentLine, startColumn, -1};
        }

        // ID = L(L|D)*  y palabras reservadas
        if (esLetra(c)) {
            size_t start = cursor;
            while (esLetraODigito(peek())) advance();
            string lexeme = source.substr(start, cursor - start);

            auto it = keywords.find(lexeme);
            if (it != keywords.end()) {
                return {it->second, lexeme, currentLine, startColumn, -1};
            }

            int pos = symbols.insert(lexeme);
            return {TOKEN_ID, lexeme, currentLine, startColumn, pos};
        }

        // TEXTO = ".*"
        if (c == '"') {
            size_t start = cursor;
            advance(); // consumir "
            while (peek() != '"' && peek() != '\n' && peek() != '\0') {
                advance();
            }
            if (peek() == '"') {
                advance(); // consumir " de cierre
                string lexeme = source.substr(start, cursor - start);
                return {TOKEN_TEXTO, lexeme, currentLine, startColumn, -1};
            }
            // cadena sin cierre -> error lexico
            string lexeme = source.substr(start, cursor - start);
            return {TOKEN_DESCONOCIDO, lexeme, currentLine, startColumn, -1};
        }

        // Operadores de comparacion / asignacion / logicos y simbolos
        // ==  =
        if (c == '=') {
            advance();
            if (peek() == '=') {
                advance();
                return {TOKEN_COMP, "==", currentLine, startColumn, -1};
            }
            return {TOKEN_ASIGN, "=", currentLine, startColumn, -1};
        }

        // !=  !
        if (c == '!') {
            advance();
            if (peek() == '=') {
                advance();
                return {TOKEN_COMP, "!=", currentLine, startColumn, -1};
            }
            return {TOKEN_NOT, "!", currentLine, startColumn, -1};
        }

        // >=  >
        if (c == '>') {
            advance();
            if (peek() == '=') {
                advance();
                return {TOKEN_COMP, ">=", currentLine, startColumn, -1};
            }
            return {TOKEN_COMP, ">", currentLine, startColumn, -1};
        }

        // <=  <
        if (c == '<') {
            advance();
            if (peek() == '=') {
                advance();
                return {TOKEN_COMP, "<=", currentLine, startColumn, -1};
            }
            return {TOKEN_COMP, "<", currentLine, startColumn, -1};
        }

        // &&
        if (c == '&') {
            advance();
            if (peek() == '&') {
                advance();
                return {TOKEN_AND, "&&", currentLine, startColumn, -1};
            }
            return {TOKEN_DESCONOCIDO, "&", currentLine, startColumn, -1};
        }

        // ||
        if (c == '|') {
            advance();
            if (peek() == '|') {
                advance();
                return {TOKEN_OR, "||", currentLine, startColumn, -1};
            }
            return {TOKEN_DESCONOCIDO, "|", currentLine, startColumn, -1};
        }

        // Operadores aritmeticos de un solo caracter
        if (c == '+') { advance(); return {TOKEN_MAS,   "+", currentLine, startColumn, -1}; }
        if (c == '-') { advance(); return {TOKEN_MENOS, "-", currentLine, startColumn, -1}; }
        if (c == '*') { advance(); return {TOKEN_POR,   "*", currentLine, startColumn, -1}; }
        if (c == '/') { advance(); return {TOKEN_DIV,   "/", currentLine, startColumn, -1}; }
        if (c == '%') { advance(); return {TOKEN_MOD,   "%", currentLine, startColumn, -1}; }

        // Simbolos especiales
        if (c == '(') { advance(); return {TOKEN_PAR_IZQ, "(", currentLine, startColumn, -1}; }
        if (c == ')') { advance(); return {TOKEN_PAR_DER, ")", currentLine, startColumn, -1}; }
        if (c == '[') { advance(); return {TOKEN_COR_IZQ, "[", currentLine, startColumn, -1}; }
        if (c == ']') { advance(); return {TOKEN_COR_DER, "]", currentLine, startColumn, -1}; }
        if (c == '{') { advance(); return {TOKEN_LLA_IZQ, "{", currentLine, startColumn, -1}; }
        if (c == '}') { advance(); return {TOKEN_LLA_DER, "}", currentLine, startColumn, -1}; }
        if (c == ',') { advance(); return {TOKEN_COMA,    ",", currentLine, startColumn, -1}; }
        if (c == ';') { advance(); return {TOKEN_PYCOMA,  ";", currentLine, startColumn, -1}; }

        // Cualquier otro caracter -> error lexico
        string lexeme(1, advance());
        return {TOKEN_DESCONOCIDO, lexeme, currentLine, startColumn, -1};
    }

    return {TOKEN_EOF, "", currentLine, currentColumn, -1};
}
