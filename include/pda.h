/**
 * Complejidad Computacional - Práctica 1
 * Simulador de un autómata con pila por vaciado de pila (APv).
 *
 * pda.h: definición formal del autómata con pila.
 */

#ifndef PDA_H_
#define PDA_H_

#include <iostream>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

#include "configuration.h"
#include "transition.h"

/**
 * Error que lanza Pda::Accepts cuando no puede decidir si una cadena pertenece
 * al lenguaje porque la búsqueda supera Pda::kMaxSteps configuraciones.
 */
class StepLimitExceeded : public std::runtime_error {
 public:
  StepLimitExceeded();
};

/**
 * Autómata con pila por vaciado de pila:
 *   M = (Q, Σ, Γ, δ, s, Z)
 */
class Pda {
 public:
  // Máximo de configuraciones que explora Accepts para una cadena. Evita que
  // la búsqueda no termine cuando una transición ε hace crecer la pila sin
  // fin, como en (q, ., S) -> (q, SS)
  static constexpr std::size_t kMaxSteps = 10000;

  // Lanza std::invalid_argument si los elementos no cumplen las restricciones
  // de la definición formal
  Pda(const std::set<std::string>& states, const std::set<char>& input_alphabet,
      const std::set<char>& stack_alphabet, const std::string& initial_state,
      char initial_stack_symbol, const std::vector<Transition>& transitions);

  const std::set<std::string>& states() const { return states_; }
  const std::set<char>& input_alphabet() const { return input_alphabet_; }
  const std::set<char>& stack_alphabet() const { return stack_alphabet_; }
  const std::string& initial_state() const { return initial_state_; }
  char initial_stack_symbol() const { return initial_stack_symbol_; }
  const std::vector<Transition>& transitions() const { return transitions_; }

  // Indica si `input` pertenece al lenguaje reconocido por el autómata. Si
  // `trace` no es nulo, escribe en él la traza: una fila por cada
  // configuración explorada, con el estado, la cadena que queda por leer, la
  // pila y las transiciones que se pueden aplicar. Lanza StepLimitExceeded si
  // la búsqueda supera kMaxSteps configuraciones
  bool Accepts(const std::string& input, std::ostream* trace = nullptr) const;

  // Transiciones que se pueden aplicar desde `configuration`
  std::vector<Transition> ApplicableTransitions(
      const Configuration& configuration) const;

 private:
  void Validate() const;

  std::set<std::string> states_;         // Q
  std::set<char> input_alphabet_;         // Σ
  std::set<char> stack_alphabet_;         // Γ
  std::string initial_state_;             // s
  char initial_stack_symbol_;             // Z
  std::vector<Transition> transitions_;   // δ
};

std::ostream& operator<<(std::ostream& os, const Pda& pda);

#endif  // PDA_H_
