/**
 * Complejidad Computacional - Práctica 1
 * Simulador de un autómata con pila por vaciado de pila (APv).
 *
 * configuration.h: descripción instantánea del autómata.
 */

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <string>

#include "transition.h"

/**
 * Situación del autómata en un instante de la simulación:
 *   (estado actual, cadena que queda por leer, contenido de la pila)
 */
class Configuration {
 public:
  Configuration(const std::string& state, const std::string& remaining,
                const std::string& stack);

  const std::string& state() const { return state_; }
  const std::string& remaining() const { return remaining_; }
  const std::string& stack() const { return stack_; }

  // Indica si `transition` se puede aplicar desde esta configuración
  bool Allows(const Transition& transition) const;

  // Configuración a la que se llega aplicando `transition`, que debe ser
  // aplicable
  Configuration Apply(const Transition& transition) const;

  // En un APv se acepta cuando se ha leído toda la cadena y la pila está vacía
  bool IsAccepting() const { return remaining_.empty() && stack_.empty(); }

  // Orden total, para poder guardar configuraciones en un std::set
  bool operator<(const Configuration& other) const;

 private:
  std::string state_;
  std::string remaining_;
  std::string stack_;  // la cima de la pila es el primer carácter
};

#endif  // CONFIGURATION_H_
