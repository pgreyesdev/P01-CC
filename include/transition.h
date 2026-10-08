/**
 * Complejidad Computacional - Práctica 1
 * Simulador de un autómata con pila por vaciado de pila (APv).
 *
 * transition.h: una transición de la función δ.
 */

#ifndef TRANSITION_H_
#define TRANSITION_H_

#include <iostream>
#include <string>

// Carácter con el que se representa ε en los ficheros
const char kEpsilon = '.';

/**
 * Representa un elemento de la función de transición:
 *   (to, push) ∈ δ(from, input, pop)
 */
class Transition {
 public:
  Transition(const std::string& from, char input, char pop,
             const std::string& to, const std::string& push);

  const std::string& from() const { return from_; }
  char input() const { return input_; }
  char pop() const { return pop_; }
  const std::string& to() const { return to_; }
  const std::string& push() const { return push_; }

  // Indica si la transición no consume símbolo de la cadena de entrada
  bool IsEpsilon() const { return input_ == kEpsilon; }

  // Indica si la transición se puede aplicar estando en el estado `state`,
  // con `remaining` como parte de la cadena aún sin leer y `top` en la cima
  // de la pila
  bool IsApplicable(const std::string& state, const std::string& remaining,
                    char top) const;

 private:
  std::string from_;  // estado de partida
  char input_;        // símbolo de Σ que se consume, o ε
  char pop_;          // símbolo de Γ que se extrae de la cima de la pila
  std::string to_;    // estado de llegada
  std::string push_;  // símbolos de Γ que se apilan, o ε (vacío)
};

std::ostream& operator<<(std::ostream& os, const Transition& transition);

#endif  // TRANSITION_H_
