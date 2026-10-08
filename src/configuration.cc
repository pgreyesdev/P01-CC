/**
 * Complejidad Computacional - Práctica 1
 * Simulador de un autómata con pila por vaciado de pila (APv).
 *
 * configuration.cc: implementación de la clase Configuration.
 */

#include "configuration.h"

#include <tuple>

Configuration::Configuration(const std::string& state,
                             const std::string& remaining,
                             const std::string& stack)
    : state_(state), remaining_(remaining), stack_(stack) {}

bool Configuration::Allows(const Transition& transition) const {
  // Con la pila vacía no hay cima que extraer: el autómata se detiene
  if (stack_.empty()) return false;
  return transition.IsApplicable(state_, remaining_, stack_.front());
}

Configuration Configuration::Apply(const Transition& transition) const {
  const std::string remaining =
      transition.IsEpsilon() ? remaining_ : remaining_.substr(1);
  // Se sustituye la cima por los símbolos apilados: el primero de ellos queda
  // como nueva cima
  const std::string stack = transition.push() + stack_.substr(1);
  return Configuration(transition.to(), remaining, stack);
}

bool Configuration::operator<(const Configuration& other) const {
  return std::tie(state_, remaining_, stack_) <
         std::tie(other.state_, other.remaining_, other.stack_);
}
