# Práctica 1: simulador de un autómata con pila

Complejidad Computacional, curso 2026/27.

Autor: Pablo García de los Reyes

## Tipo de autómata implementado

**Autómata con pila por vaciado de pila (APv).**

Una cadena pertenece al lenguaje si existe alguna secuencia de transiciones que
consuma toda la cadena y deje la pila vacía. El autómata no tiene conjunto de
estados finales, así que el fichero de definición no lleva la línea del
conjunto F.

## Requisitos

- Compilador de C++ con soporte de C++17 (probado con GCC).
- CMake 3.16 o superior.

## Compilación

Desde la carpeta del proyecto:

```
cmake -S . -B build
cmake --build build
```

El ejecutable queda en `build/pda` (`build\pda.exe` en Windows).

## Ejecución

```
pda -config <f> -trace <y|n> [-in <f>] [-out <f>]
```

| Opción | Descripción |
|---|---|
| `-config <f>` | Fichero de texto con la definición del autómata. Obligatoria. |
| `-trace <y\|n>` | Indica si se muestra la traza. Obligatoria. |
| `-in <f>` | Fichero con las cadenas a comprobar, una por línea. Si no se indica, se leen de teclado. |
| `-out <f>` | Fichero donde se escribe la traza. Si no se indica, se muestra por pantalla. |

Las opciones pueden ir en cualquier orden. Una línea vacía representa la cadena
vacía. Al leer de teclado, la entrada termina con Ctrl+D (Linux) o Ctrl+Z y
Enter (Windows).

Ejemplos:

```
build/pda -config data/APv-1.txt -trace n
build/pda -config data/APv-1.txt -trace y -in data/cadenas-1.txt
build/pda -config data/APv-2.txt -trace y -in data/cadenas-2.txt -out traza.txt
```

## Formato del fichero del autómata

```
# Comentarios
q1 q2 q3 ...   # conjunto Q
a1 a2 a3 ...   # conjunto Σ
A1 A2 A3 ...   # conjunto Γ
q1             # estado inicial
A1             # símbolo inicial de la pila
q1 a A1 q2 A   # transición: (q2, A) ∈ δ(q1, a, A1)
...            # una transición por línea
```

- Los símbolos de Σ y de Γ son de un único carácter.
- ε se escribe con un punto (`.`), tanto en el símbolo de entrada como en la
  cadena que se apila.
- La cadena que se apila se escribe sin espacios y su primer símbolo queda en
  la cima de la pila.

Al leer el fichero se comprueba que cumple la definición formal: Q no está
vacío, el estado inicial pertenece a Q, el símbolo inicial de la pila pertenece
a Γ, y los estados y símbolos de cada transición pertenecen a Q, Σ y Γ. Si no
se cumple, el programa muestra el error y termina.

## Salida

Para cada cadena se indica si pertenece o no al lenguaje:

```
'aabb': pertenece
'aab': no pertenece
```

Con `-trace y` se muestra además una fila por cada configuración explorada, con
el estado, la cadena que queda por leer, la pila (cima a la izquierda) y las
transiciones que se pueden aplicar desde ella:

```
Cadena 'ab':
Estado  Cadena  Pila      Transiciones
q1      ab      S         (q1, a, S) -> (q1, A)
q1      b       A         (q1, b, A) -> (q2, .)
q2      .       .         cadena aceptada
```

Como el autómata puede ser no determinista, la simulación hace una búsqueda en
profundidad: si una rama no lleva a la aceptación, la traza continúa con la
siguiente configuración pendiente.

### Límite de configuraciones

Un autómata con una transición ε que haga crecer la pila sin leer entrada, como
`q . S q SS`, genera infinitas configuraciones distintas y la búsqueda no
terminaría. Para evitarlo se exploran como máximo 10000 configuraciones por
cadena (`Pda::kMaxSteps`). Si se supera, el programa lo indica y continúa con
la siguiente cadena:

```
'ab': no se pudo decidir, se supero el limite de 10000 configuraciones exploradas
```

## Estructura del proyecto

```
CMakeLists.txt
include/          cabeceras
src/              implementación
data/             autómatas de ejemplo (APv-*.txt) y cadenas de prueba (cadenas-*.txt)
```

| Clase | Responsabilidad |
|---|---|
| `Transition` | Una transición de δ. |
| `Configuration` | Descripción instantánea: estado, cadena por leer y pila. |
| `Pda` | Definición formal del autómata, su validación y la simulación. |
| `PdaParser` | Lectura del fichero de definición. |
| `Options` | Opciones de la línea de comandos. |
