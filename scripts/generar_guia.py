"""
Genera un documento .docx con la guia de los 11 pasos del Entregable 1
del Analizador Sintactico LP, incluyendo un mapa mental de contexto.

Ejecucion:
    python scripts/generar_guia.py
Salida:
    Guia_Entregable1_AnalizadorSintactico.docx (en la raiz del proyecto)
"""
from docx import Document
from docx.shared import Pt, RGBColor, Cm, Inches
from docx.enum.text import WD_ALIGN_PARAGRAPH
from docx.enum.table import WD_ALIGN_VERTICAL
from docx.oxml.ns import qn
from docx.oxml import OxmlElement

doc = Document()

# ===== Estilos base =====
style = doc.styles['Normal']
style.font.name = 'Calibri'
style.font.size = Pt(11)

def set_cell_bg(cell, color_hex):
    tc_pr = cell._tc.get_or_add_tcPr()
    shd = OxmlElement('w:shd')
    shd.set(qn('w:val'), 'clear')
    shd.set(qn('w:color'), 'auto')
    shd.set(qn('w:fill'), color_hex)
    tc_pr.append(shd)

def add_heading(text, level=1, color=None):
    h = doc.add_heading(text, level=level)
    if color:
        for run in h.runs:
            run.font.color.rgb = RGBColor.from_string(color)
    return h

def add_para(text, bold=False, italic=False, size=11):
    p = doc.add_paragraph()
    r = p.add_run(text)
    r.font.size = Pt(size)
    r.bold = bold
    r.italic = italic
    return p

def add_bullet(text, level=0):
    p = doc.add_paragraph(text, style='List Bullet')
    p.paragraph_format.left_indent = Cm(0.6 + level * 0.6)
    return p

def add_code(text):
    p = doc.add_paragraph()
    r = p.add_run(text)
    r.font.name = 'Consolas'
    r.font.size = Pt(9)
    p.paragraph_format.left_indent = Cm(0.5)
    return p

# =============================================================
# PORTADA
# =============================================================
title = doc.add_paragraph()
title.alignment = WD_ALIGN_PARAGRAPH.CENTER
r = title.add_run('Guía del Analizador Sintáctico LP')
r.bold = True
r.font.size = Pt(22)

sub = doc.add_paragraph()
sub.alignment = WD_ALIGN_PARAGRAPH.CENTER
r = sub.add_run('Entregable 1 (30%) — Compiladores 2026-II')
r.font.size = Pt(14)
r.italic = True

meta = doc.add_paragraph()
meta.alignment = WD_ALIGN_PARAGRAPH.CENTER
meta.add_run('Grupo 6 · Fecha de entrega: 13 de noviembre de 2026\n').font.size = Pt(11)
meta.add_run('Documento guía: resumen de los 11 pasos de implementación').font.size = Pt(11)

doc.add_paragraph()
doc.add_paragraph('_' * 70).alignment = WD_ALIGN_PARAGRAPH.CENTER

# =============================================================
# 1. CONTEXTO
# =============================================================
add_heading('1. Contexto del proyecto', 1, color='1F4E79')

add_para(
    'El curso de Compiladores pide construir, en C++, un compilador para el '
    'lenguaje LP. El trabajo se divide en dos grandes bloques:'
)
add_bullet('Analizador léxico (Lexer): ya entregado en fases anteriores. Lee el archivo .lp y produce una secuencia de tokens.')
add_bullet('Analizador sintáctico (Parser): es lo que estamos construyendo ahora. Toma los tokens y verifica que la estructura del programa cumpla la gramática LP.')

add_para(
    'El analizador sintáctico se entrega en dos fases: Entregable 1 (30%, 13-nov) '
    'y Entregable 2 (70%, 24-nov). Este documento cubre únicamente el Entregable 1.',
    italic=True
)

add_heading('¿Qué pide el Entregable 1?', 2, color='2E75B6')
add_bullet('Integrar el lexer con el parser (leer .lp → tokens → analizar).')
add_bullet('Reconocer la estructura básica: void main(), bloque { }, declaraciones y asignaciones simples.')
add_bullet('Detectar errores básicos: punto y coma faltante, paréntesis/llaves sin cerrar, token inesperado.')
add_bullet('Dar un mensaje de error comprensible (ubicación, token esperado, token encontrado).')

# =============================================================
# 2. MAPA MENTAL
# =============================================================
doc.add_page_break()
add_heading('2. Mapa mental — Estructura del proyecto', 1, color='1F4E79')

add_para(
    'El siguiente mapa muestra cómo encajan los módulos. El flujo va de izquierda '
    'a derecha: el archivo .lp entra por el lexer y sale como veredicto del parser.',
    italic=True
)

# Flujo general como tabla
add_heading('2.1 Flujo general', 2, color='2E75B6')
flow = doc.add_table(rows=1, cols=5)
flow.style = 'Light Grid Accent 1'
celdas = ['Archivo .lp', '→ Lexer', '→ Tokens', '→ Parser', '→ ACEPTADO / ERRORES']
for i, txt in enumerate(celdas):
    c = flow.rows[0].cells[i]
    c.text = txt
    for p in c.paragraphs:
        p.alignment = WD_ALIGN_PARAGRAPH.CENTER
        for r in p.runs:
            r.bold = True

# Jerarquia de archivos
add_heading('2.2 Archivos del proyecto', 2, color='2E75B6')
tabla = doc.add_table(rows=1, cols=3)
tabla.style = 'Light Grid Accent 1'
hdr = tabla.rows[0].cells
hdr[0].text = 'Archivo'
hdr[1].text = 'Rol'
hdr[2].text = 'Estado'
for c in hdr:
    for p in c.paragraphs:
        for r in p.runs:
            r.bold = True
    set_cell_bg(c, 'D9E2F3')

filas = [
    ('src/Token.h', 'Enum de tipos de token + struct Token (lexema, línea, columna)', 'Modificado (añadido TOKEN_FUNC)'),
    ('src/Lexer.h', 'Interfaz del lexer', 'Comentario añadido'),
    ('src/Lexer.cpp', 'Implementación del lexer', 'Modificado (añadido "func" como keyword)'),
    ('src/SymbolTable.{h,cpp}', 'Tabla de identificadores', 'Sin cambios'),
    ('src/Parser.h', 'Interfaz del parser (declaraciones)', 'NUEVO'),
    ('src/Parser.cpp', 'Implementación del parser (descenso recursivo)', 'NUEVO'),
    ('src/Main.cpp', 'Punto de entrada: carga archivo, llama parser', 'Reescrito'),
    ('.vscode/tasks.json', 'Compilación con Ctrl+Shift+B', 'Modificado'),
    ('README.md', 'Instrucciones y catálogo de pruebas', 'Reescrito'),
    ('tests/valido1.lp, valido2.lp', 'Casos válidos de prueba', 'NUEVO'),
    ('tests/invalido1.lp, invalido2.lp, invalido3.lp', 'Casos inválidos de prueba', 'NUEVO'),
]
for fila in filas:
    row = tabla.add_row().cells
    for i, v in enumerate(fila):
        row[i].text = v

# Jerarquia conceptual del parser
add_heading('2.3 Jerarquía conceptual del parser (descenso recursivo)', 2, color='2E75B6')
add_para(
    'Cada no terminal de la gramática se implementa como una función en Parser.cpp. '
    'Las funciones se llaman unas a otras siguiendo la jerarquía:'
)
add_code('parsePrograma()')
add_code('  └── parsePrincipal()              [void] main ( ) BLOQUE')
add_code('        └── parseBloque()           { LISTA_VARIABLES COMANDOS }')
add_code('              ├── parseListaVariables()')
add_code('              │     ├── parseTipo()')
add_code('              │     │     ├── parseTipoBase()   int|float|char|boolean')
add_code('              │     │     └── parseDimension()  [ num_int ] ...')
add_code('              │     └── parseListaId()          , id , id ...')
add_code('              └── parseComandos()')
add_code('                    └── parseComando()')
add_code('                          └── parseAsignacion()  id = COMPLEMENTO ;')
add_code('                                └── parseComplemento()')
add_code('                                      └── parseExpresionSimple()')

# =============================================================
# 3. LOS 11 PASOS
# =============================================================
doc.add_page_break()
add_heading('3. Los 11 pasos de implementación', 1, color='1F4E79')
add_para(
    'Cada paso incluye: qué se hizo, por qué, archivos afectados y qué recordar. '
    'Se recomienda leerlos en orden.',
    italic=True
)

pasos = [
    {
        'num': 1,
        'titulo': 'Añadir "func" como palabra reservada en el lexer',
        'por_que': ('La gramática LP tiene la producción FUNCION → func id ( ARGUMENTOS ). '
                    'Si no agregamos "func" al lexer, lo reconocería como ID genérico y '
                    'el parser nunca podría distinguir una llamada a función.'),
        'archivos': ['src/Token.h (nuevo TOKEN_FUNC)', 'src/Lexer.cpp (keywords["func"] = TOKEN_FUNC)'],
        'recordar': 'El uso real de func llega en el Entregable 2, pero el token debe existir desde ya.',
    },
    {
        'num': 2,
        'titulo': 'Exponer el lexer para que entregue tokens al parser',
        'por_que': ('El parser necesita pedir tokens uno a uno sin tocar el estado interno del '
                    'lexer. El método público getNextToken() ya cumplía ese contrato, por lo '
                    'que solo añadimos un comentario que lo deja explícito.'),
        'archivos': ['src/Lexer.h (solo comentario)'],
        'recordar': 'El parser nunca toca la cadena fuente ni el cursor del lexer; solo consume tokens.',
    },
    {
        'num': 3,
        'titulo': 'Crear Parser.h (declaración de la clase)',
        'por_que': ('Define el contrato: qué métodos existen, qué atributos tiene, qué puede '
                    'llamarse desde Main. Separar interfaz de implementación es buena práctica '
                    'y permite que otros archivos usen el parser sin ver su código.'),
        'archivos': ['src/Parser.h (NUEVO)'],
        'recordar': ('Hay una función por cada no terminal de la gramática del Entregable 1. '
                     'Cada declaración incluye la producción en comentario para no abrir el PDF.'),
    },
    {
        'num': 4,
        'titulo': 'Crear Parser.cpp (implementación del descenso recursivo)',
        'por_que': ('Es el corazón del proyecto. Cada función parseXxx() consume tokens hasta '
                    'cerrar su producción y decide qué alternativa aplicar mirando el token '
                    'actual (lookahead de 1 token, por eso la gramática debe ser LL(1)).'),
        'archivos': ['src/Parser.cpp (NUEVO)'],
        'recordar': ('Si una función ve un token que no esperaba, llama a error() y sigue. '
                     'Esto permite reportar varios errores en una sola corrida.'),
    },
    {
        'num': 5,
        'titulo': 'Diseñar el manejo de errores (3 decisiones clave)',
        'por_que': ('Un parser debe reportar errores comprensibles sin detenerse al primero. '
                    'Las decisiones tomadas: (a) error() imprime pero no lanza excepción; '
                    '(b) expect() NO avanza en error, para no perder el token siguiente; '
                    '(c) parseExpresionSimple() SÍ avanza en error, para no quedar en bucle.'),
        'archivos': ['src/Parser.cpp (integrado en el paso 4)'],
        'recordar': ('Regla general: si falta un delimitador (;, ), }) no avanzar; si falta un '
                     'operando central, sí avanzar para evitar loops.'),
    },
    {
        'num': 6,
        'titulo': 'Integrar el parser en Main.cpp',
        'por_que': ('El parser existía como clase pero nadie lo instanciaba. Main carga el '
                    'archivo, crea el Lexer, se lo pasa al Parser por referencia y llama a parse(). '
                    'La bandera --tokens permite seguir viendo la lista de tokens para debug.'),
        'archivos': ['src/Main.cpp (reescrito)'],
        'recordar': ('Si usas --tokens se crean DOS lexers: uno para imprimir tokens, otro para '
                     'el parser. Un mismo lexer no puede hacer las dos cosas porque getNextToken '
                     'consume los tokens.'),
    },
    {
        'num': 7,
        'titulo': 'Crear archivos .lp de prueba (válidos e inválidos)',
        'por_que': ('El PDF pide "al menos un programa válido y dos casos inválidos" como '
                    'evidencia. Creamos 2 válidos (cubriendo más producciones) y 3 inválidos '
                    'cubriendo los tres errores típicos: ; faltante, { sin cerrar, ( sin cerrar.'),
        'archivos': ['tests/valido1.lp', 'tests/valido2.lp',
                     'tests/invalido1.lp', 'tests/invalido2.lp', 'tests/invalido3.lp'],
        'recordar': ('OJO: en LP la dimensión del arreglo va con el tipo. Se escribe '
                     '"float[10] notas" y NO "float notas[10]" (sintaxis de C).'),
    },
    {
        'num': 8,
        'titulo': 'Actualizar README.md',
        'por_que': ('Evidencia obligatoria del Entregable 1. Debe permitir compilar y ejecutar '
                    'el proyecto sin preguntar nada. Incluye una tabla de cobertura que mapea '
                    'cada requisito del PDF a la función concreta del parser (útil para la '
                    'sustentación).'),
        'archivos': ['README.md (reescrito)'],
        'recordar': ('Documenta las 5 "trampas" de la gramática LP: void opcional, dimensión '
                     'antes del id, func reservado, return exige expresión, no hay for.'),
    },
    {
        'num': 9,
        'titulo': 'Actualizar .vscode/tasks.json',
        'por_que': ('Para que Ctrl+Shift+B compile también Parser.cpp. Sin este cambio la tarea '
                    'de VS Code daría error de "undefined reference" porque Main.cpp usa '
                    'funciones del parser que no estarían enlazadas.'),
        'archivos': ['.vscode/tasks.json', 'build/ (carpeta creada)'],
        'recordar': 'Hay que crear la carpeta build/ a mano la primera vez.',
    },
    {
        'num': 10,
        'titulo': 'Probar todo end-to-end',
        'por_que': ('Validar que el flujo completo funciona: compilar limpio, correr los 5 '
                    'archivos de prueba, verificar que válidos aceptan (exit 0) e inválidos '
                    'rechazan (exit 1) con el mensaje correcto.'),
        'archivos': ['(solo ejecución)'],
        'recordar': ('Resultados: 2 válidos aceptados, 3 inválidos rechazados con un error claro '
                     'cada uno. Mensajes con línea, columna, token esperado y token encontrado.'),
    },
    {
        'num': 11,
        'titulo': 'Commit en git',
        'por_que': ('Dejar registro de la fase 4 (sintáctico - Entregable 1) separada de las '
                    'fases léxicas anteriores. Permite a los integrantes y al profesor ver qué '
                    'cambió exactamente para este entregable.'),
        'archivos': ['(git add + git commit)'],
        'recordar': 'Mensaje sugerido: "feat: analizador sintáctico Entregable 1 (30%)".',
    },
]

for p in pasos:
    add_heading(f"Paso {p['num']}: {p['titulo']}", 2, color='2E75B6')
    add_para('¿Por qué?', bold=True)
    add_para(p['por_que'])
    add_para('Archivos afectados:', bold=True)
    for a in p['archivos']:
        add_bullet(a)
    add_para('Qué recordar:', bold=True)
    add_para(p['recordar'], italic=True)
    doc.add_paragraph()

# =============================================================
# 4. RESUMEN DE EVIDENCIAS
# =============================================================
doc.add_page_break()
add_heading('4. Resumen de evidencias para la entrega', 1, color='1F4E79')
add_para('Según la sección 4.4 del PDF de especificaciones:')
checklist = [
    ('Código fuente del proyecto, incluyendo la integración con el lexer', 'LISTO (src/)'),
    ('Archivos .lp utilizados para las pruebas', 'LISTO (tests/)'),
    ('Evidencia de ejecución: 1 programa válido + 2 inválidos (mínimo)', 'LISTO (2 válidos + 3 inválidos)'),
    ('Breve README con instrucciones de compilación y ejecución', 'LISTO (README.md)'),
    ('Sustentación: cada integrante explica una parte del flujo', 'PENDIENTE (en vivo)'),
]
tabla_ev = doc.add_table(rows=1, cols=2)
tabla_ev.style = 'Light Grid Accent 1'
hdr = tabla_ev.rows[0].cells
hdr[0].text = 'Evidencia pedida'
hdr[1].text = 'Estado'
for c in hdr:
    for p in c.paragraphs:
        for r in p.runs:
            r.bold = True
    set_cell_bg(c, 'D9E2F3')
for ev, st in checklist:
    row = tabla_ev.add_row().cells
    row[0].text = ev
    row[1].text = st

# =============================================================
# 5. GUIA RAPIDA PARA LA SUSTENTACION
# =============================================================
add_heading('5. Guía rápida para la sustentación', 1, color='1F4E79')
add_para(
    'Cada integrante del equipo puede ser consultado sobre cualquier módulo. '
    'Estas son las 5 preguntas más probables y dónde apuntar en el código:',
    italic=True
)

preguntas = [
    ('¿Dónde reconoces el bloque { }?',
     'En Parser.cpp, función parseBloque(): expect LLA_IZQ, luego parseListaVariables y parseComandos, luego expect LLA_DER.'),
    ('¿Cómo detectas que falta un punto y coma?',
     'En parseListaVariables y parseAsignacion llamamos a expect(TOKEN_PYCOMA). Si el token actual no es ;, se llama a error() con mensaje y ubicación.'),
    ('¿Qué pasa si el parser encuentra un error?',
     'Se imprime el error (línea, columna, esperado, encontrado) y se incrementa errorCount, pero NO se detiene. Esto permite reportar varios errores en una corrida.'),
    ('¿Por qué separaste parser.h y parser.cpp?',
     'Para separar interfaz de implementación: Main solo necesita ver qué métodos existen (el .h), no cómo están implementados (el .cpp).'),
    ('¿Qué queda para el Entregable 2?',
     'Expresiones completas con precedencia (||, &&, comp, +-, */ %), if/else/while, scanf/println, funciones (declaración y llamadas con func), return, y la construcción del AST.'),
]
for i, (q, r) in enumerate(preguntas, 1):
    add_para(f'P{i}. {q}', bold=True)
    add_para(r)

# =============================================================
# SAVE
# =============================================================
out = r'D:\01_Universidad\2026_II\Compiladores\Guia_Entregable1_AnalizadorSintactico.docx'
doc.save(out)
print(f'OK: {out}')
