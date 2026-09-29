# Compiladores
Evidencias Grupo 6
08/09/2026
  Fecha de entrega 01/12/2026 para Ev4 
11/009/2026
Objetivo
Implementar en C++ un analizador léxico para el lenguaje LP:
- Reconocer los tokens definidos en la lista de las expresiones regulares proporcionadas; 
- Generar una lista secuencial de tokens, mantener una tabla de símbolos con identificadores únicos y reportar errores léxicos.

Consultar el material en anexo sobre los detalles de implementacion: Proyecto_LexLP_Analizador_Lexico. 

Entregas: en 3 fases
- Presentar cada fase semanalmente, los martes en el laboratorio, a partir de 15/09
  - 15/09: Lectura + NUM_INT + NUM_DEC
  - 22/09: ID + TEXTO + palabras reservadas + tabla de símbolos
  - 29/09: operadores + Tokens completos + Integración + lista de tokens + errores + pruebas + presentación

## Estructura del proyecto
```
src/     Código fuente (Main, Lexer, SymbolTable, Token)
tests/   Archivos de entrada de prueba (.txt, .lp)
docs/    Material del curso (enunciado, expresiones regulares)
build/   Ejecutable generado (no se sube al repositorio)
```

## Compilar y ejecutar
```
g++ -g src/Main.cpp src/Lexer.cpp src/SymbolTable.cpp -o build/Main.exe
build/Main.exe tests/prueba_semana2.lp
```
En VS Code: `Ctrl+Shift+B` ejecuta la tarea "Compilar LexLP".
