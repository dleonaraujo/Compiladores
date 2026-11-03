# Compiladores
Evidencias Grupo 6

## Objetivo
Implementar en C++ un compilador para el lenguaje LP, compuesto por:
1. **Analizador léxico (Lexer)** — ya entregado (fases 1 a 3).
2. **Analizador sintáctico (Parser)** — en curso: dos entregables (30% + 70%).

## Estructura del proyecto
```
src/     Código fuente (Main, Lexer, SymbolTable, Token, Parser)
tests/   Archivos de entrada de prueba (.lp, .txt)
docs/    Material del curso (enunciado léxico, expresiones regulares)
```

Archivos de referencia en la raíz:
- `Gramatica_LP_LL1.pdf` — gramática LL(1) de LP.
- `Especificaciones_Proyecto-Analizador_Sintactico_AL.pdf` — reglas de entrega.

## Compilar
```
g++ -g src/Main.cpp src/Lexer.cpp src/SymbolTable.cpp src/Parser.cpp -o src/Main.exe
```
En VS Code: `Ctrl+Shift+B` ejecuta la tarea "Compilar LexLP".

## Ejecutar

### Análisis sintáctico (modo por defecto)
```
./src/Main.exe tests/valido1.lp
```
Imprime si el programa es **ACEPTADO** o **RECHAZADO** y la tabla de símbolos.

### Lista de tokens + análisis sintáctico
```
./src/Main.exe tests/valido1.lp --tokens
```
Imprime primero la lista de tokens (evidencia del lexer) y luego el resultado del parser.

## Código de salida
- `0` → programa aceptado sintácticamente.
- `1` → errores sintácticos o problema al abrir el archivo.

## Archivos de prueba (`tests/`)

### Válidos
- `valido1.lp` — ejemplo mínimo del PDF (declaraciones + asignaciones).
- `valido2.lp` — múltiples IDs por declaración, arreglos (`float[10] notas`), varios tipos base.

### Inválidos (se espera que el parser los rechace)
- `invalido1.lp` — falta `;` al final de una declaración (ejemplo del PDF).
- `invalido2.lp` — llave `{` sin cerrar.
- `invalido3.lp` — paréntesis `(` sin cerrar en `main`.

### Cobertura del Entregable 1 (según sección 4.1 del PDF)
| Requisito | Dónde se cubre |
|---|---|
| Integración lexer→parser | `Main.cpp` + `Parser(Lexer&)` |
| `PROGRAMA` y `PRINCIPAL` | `parsePrograma`, `parsePrincipal` |
| `BLOQUE` con llaves | `parseBloque` |
| Declaraciones (`TIPO`, `TIPO_BASE`, `DIMENSION`, `LISTA_VARIABLES`, `LISTA_ID`) | `parseTipo`, `parseTipoBase`, `parseDimension`, `parseListaVariables`, `parseListaId` |
| Asignaciones básicas (`ASIGNACION`, `COMPLEMENTO`) | `parseAsignacion`, `parseComplemento` |
| Errores: `;` faltante, paréntesis/llaves sin cerrar, token inesperado | `expect()`, `error()` |

## Notas sobre la gramática LP
- **`void` opcional antes de `main`**: la gramática formal pide `main() BLOQUE`, pero el ejemplo del PDF usa `void main()`. El parser acepta ambos.
- **Dimensión de arreglos va con el TIPO**: en LP se escribe `float[10] notas;`, NO `float notas[10];` (ver producción `TIPO → TIPO_BASE DIMENSION`).
- **`func` es palabra reservada**: añadido al lexer para la producción `FUNCION → func id ( ARGUMENTOS )`. Se usará recién en el Entregable 2.
- **`return` exige expresión**: la gramática no admite `return;` solo. Se implementa en el Entregable 2.
- **No hay `for`**: la gramática solo define `while`.

## Entregas
### Entregables léxico (ya cerrados)
- 15/09: lectura + NUM_INT + NUM_DEC
- 22/09: ID + TEXTO + palabras reservadas + tabla de símbolos
- 29/09: operadores + integración + lista de tokens + errores

### Entregables sintáctico (en curso)
- **13/11 — Entregable 1 (30%)**: integración lexer+parser, `main`/bloque/declaraciones/asignaciones + errores básicos.
- **24/11 — Entregable 2 (70%)**: expresiones completas con precedencia, `if`/`else`/`while`, `scanf`/`println`, funciones, `return`, AST, mejor recuperación de errores.
