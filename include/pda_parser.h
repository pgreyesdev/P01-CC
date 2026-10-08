/**
 * Complejidad Computacional - Práctica 1
 * Simulador de un autómata con pila por vaciado de pila (APv).
 *
 * pda_parser.h: lectura de la definición de un autómata desde un fichero.
 */

#ifndef PDA_PARSER_H_
#define PDA_PARSER_H_

#include <iostream>
#include <set>
#include <string>
#include <vector>

#include "pda.h"

/**
 * Lee la definición de un APv con el formato:
 *   # Comentarios
 *   q1 q2 q3 ...     # conjunto Q
 *   a1 a2 a3 ...     # conjunto Σ
 *   A1 A2 A3 ...     # conjunto Γ
 *   q1               # estado inicial
 *   A1               # símbolo inicial de la pila
 *   q1 a A1 q2 A     # una transición por línea
 */
class PdaParser {
 public:
  explicit PdaParser(std::istream& input);

  // Lanza std::runtime_error si el formato no es correcto y
  // std::invalid_argument si el autómata leído no es válido
  Pda Parse();

 private:
  // Lee la siguiente línea con contenido, sin comentarios y separada en
  // palabras. Devuelve false si se ha llegado al final de la entrada
  bool NextLine(std::vector<std::string>& tokens);

  // Como NextLine, pero es un error que no queden líneas. `what` describe lo
  // que se esperaba leer
  std::vector<std::string> RequireLine(const std::string& what);

  // Igual que RequireLine, pero la línea debe tener una única palabra
  std::string RequireSingle(const std::string& what);

  std::set<char> ToSymbols(const std::vector<std::string>& tokens) const;
  char ToSymbol(const std::string& token) const;

  [[noreturn]] void Fail(const std::string& message) const;

  std::istream& input_;
  int line_number_ = 0;  // línea actual, para los mensajes de error
};

#endif  // PDA_PARSER_H_
