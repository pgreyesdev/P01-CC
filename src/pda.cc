/**
 * Complejidad Computacional - Práctica 1
 * Simulador de un autómata con pila por vaciado de pila (APv).
 *
 * pda.cc: implementación de la clase Pda.
 */

#include "pda.h"

#include <algorithm>
#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace {

// Escribe los elementos de un conjunto separados por comas y entre llaves
template <typename T>
void PrintSet(std::ostream& os, const std::set<T>& elements) {
  os << "{";
  bool first = true;
  for (const T& element : elements) {
    if (!first) os << ", ";
    os << element;
    first = false;
  }
  os << "}";
}

// Escribe una fila de la traza. La cadena y la pila vacías se muestran como ε
void PrintRow(std::ostream& os, std::size_t state_width,
              std::size_t input_width, const std::string& state,
              const std::string& remaining, const std::string& stack,
              const std::string& transitions) {
  const std::string epsilon(1, kEpsilon);
  os << std::left << std::setw(state_width) << state << "  "
     << std::setw(input_width) << (remaining.empty() ? epsilon : remaining)
     << "  " << std::setw(input_width + 2) << (stack.empty() ? epsilon : stack)
     << "  " << transitions << "\n";
}

}  // namespace

StepLimitExceeded::StepLimitExceeded()
    : std::runtime_error("no se pudo decidir, se supero el limite de " +
                         std::to_string(Pda::kMaxSteps) +
                         " configuraciones exploradas") {}

Pda::Pda(const std::set<std::string>& states,
         const std::set<char>& input_alphabet,
         const std::set<char>& stack_alphabet, const std::string& initial_state,
         char initial_stack_symbol, const std::vector<Transition>& transitions)
    : states_(states),
      input_alphabet_(input_alphabet),
      stack_alphabet_(stack_alphabet),
      initial_state_(initial_state),
      initial_stack_symbol_(initial_stack_symbol),
      transitions_(transitions) {
  Validate();
}

bool Pda::Accepts(const std::string& input, std::ostream* trace) const {
  const Configuration initial(initial_state_, input,
                              std::string(1, initial_stack_symbol_));
  // Anchos de las columnas de la traza
  std::size_t state_width = 6;
  for (const std::string& state : states_) {
    state_width = std::max(state_width, state.size());
  }
  const std::size_t input_width = std::max<std::size_t>(6, input.size());
  if (trace != nullptr) {
    PrintRow(*trace, state_width, input_width, "Estado", "Cadena", "Pila",
             "Transiciones");
  }
  // Búsqueda en profundidad de una secuencia de transiciones que lleve a una
  // configuración de aceptación. `pending` guarda las configuraciones que
  // quedan por explorar y `visited` las ya exploradas, para no repetirlas
  std::vector<Configuration> pending = {initial};
  std::set<Configuration> visited;
  while (!pending.empty()) {
    const Configuration current = pending.back();
    pending.pop_back();
    if (current.IsAccepting()) {
      if (trace != nullptr) {
        PrintRow(*trace, state_width, input_width, current.state(),
                 current.remaining(), current.stack(), "cadena aceptada");
      }
      return true;
    }
    if (!visited.insert(current).second) continue;
    if (visited.size() > kMaxSteps) throw StepLimitExceeded();
    // Se apilan en orden inverso para explorar primero la primera transición
    const std::vector<Transition> applicable = ApplicableTransitions(current);
    if (trace != nullptr) {
      std::ostringstream options;
      for (std::size_t i = 0; i < applicable.size(); ++i) {
        if (i > 0) options << ", ";
        options << applicable[i];
      }
      PrintRow(*trace, state_width, input_width, current.state(),
               current.remaining(), current.stack(),
               applicable.empty() ? "ninguna" : options.str());
    }
    for (auto it = applicable.rbegin(); it != applicable.rend(); ++it) {
      pending.push_back(current.Apply(*it));
    }
  }
  return false;
}

std::vector<Transition> Pda::ApplicableTransitions(
    const Configuration& configuration) const {
  std::vector<Transition> applicable;
  for (const Transition& transition : transitions_) {
    if (configuration.Allows(transition)) applicable.push_back(transition);
  }
  return applicable;
}

void Pda::Validate() const {
  if (states_.empty()) {
    throw std::invalid_argument("el conjunto de estados Q esta vacio");
  }
  if (input_alphabet_.count(kEpsilon) > 0) {
    throw std::invalid_argument(
        "el simbolo '.' esta reservado para epsilon y no puede estar en Sigma");
  }
  if (stack_alphabet_.count(kEpsilon) > 0) {
    throw std::invalid_argument(
        "el simbolo '.' esta reservado para epsilon y no puede estar en Gamma");
  }
  if (states_.count(initial_state_) == 0) {
    throw std::invalid_argument("el estado inicial '" + initial_state_ +
                                "' no pertenece a Q");
  }
  if (stack_alphabet_.count(initial_stack_symbol_) == 0) {
    throw std::invalid_argument(std::string("el simbolo inicial de la pila '") +
                                initial_stack_symbol_ +
                                "' no pertenece a Gamma");
  }
  for (const Transition& transition : transitions_) {
    std::ostringstream where;
    where << "en la transicion " << transition << ": ";
    if (states_.count(transition.from()) == 0) {
      throw std::invalid_argument(where.str() + "el estado '" +
                                  transition.from() + "' no pertenece a Q");
    }
    if (states_.count(transition.to()) == 0) {
      throw std::invalid_argument(where.str() + "el estado '" +
                                  transition.to() + "' no pertenece a Q");
    }
    if (!transition.IsEpsilon() &&
        input_alphabet_.count(transition.input()) == 0) {
      throw std::invalid_argument(where.str() + "el simbolo '" +
                                  transition.input() +
                                  "' no pertenece a Sigma");
    }
    if (stack_alphabet_.count(transition.pop()) == 0) {
      throw std::invalid_argument(where.str() + "el simbolo '" +
                                  transition.pop() + "' no pertenece a Gamma");
    }
    for (char symbol : transition.push()) {
      if (stack_alphabet_.count(symbol) == 0) {
        throw std::invalid_argument(where.str() + "el simbolo '" + symbol +
                                    "' no pertenece a Gamma");
      }
    }
  }
}

std::ostream& operator<<(std::ostream& os, const Pda& pda) {
  os << "Q = ";
  PrintSet(os, pda.states());
  os << "\nSigma = ";
  PrintSet(os, pda.input_alphabet());
  os << "\nGamma = ";
  PrintSet(os, pda.stack_alphabet());
  os << "\ns = " << pda.initial_state();
  os << "\nZ = " << pda.initial_stack_symbol();
  os << "\nTransiciones:\n";
  for (const Transition& transition : pda.transitions()) {
    os << "  " << transition << "\n";
  }
  return os;
}
