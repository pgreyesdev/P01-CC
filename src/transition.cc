/**
 * Complejidad Computacional - Práctica 1
 * Simulador de un autómata con pila por vaciado de pila (APv).
 *
 * transition.cc: implementación de la clase Transition.
 */

#include "transition.h"

Transition::Transition(const std::string& from, char input, char pop,
                       const std::string& to, const std::string& push)
    : from_(from), input_(input), pop_(pop), to_(to) {
  // Apilar ε es no apilar nada, así que se guarda como cadena vacía
  if (push != std::string(1, kEpsilon)) push_ = push;
}

bool Transition::IsApplicable(const std::string& state,
                              const std::string& remaining, char top) const {
  if (state != from_ || top != pop_) return false;
  if (IsEpsilon()) return true;
  return !remaining.empty() && remaining.front() == input_;
}

std::ostream& operator<<(std::ostream& os, const Transition& transition) {
  os << "(" << transition.from() << ", " << transition.input() << ", "
     << transition.pop() << ") -> (" << transition.to() << ", ";
  if (transition.push().empty()) {
    os << kEpsilon;
  } else {
    os << transition.push();
  }
  return os << ")";
}
