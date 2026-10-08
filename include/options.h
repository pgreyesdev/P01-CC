/**
 * Complejidad Computacional - Práctica 1
 * Simulador de un autómata con pila por vaciado de pila (APv).
 *
 * options.h: opciones de la línea de comandos.
 */

#ifndef OPTIONS_H_
#define OPTIONS_H_

#include <string>

/**
 * Opciones del programa:
 *   -config <f> -trace <y|n> [-in <f>] [-out <f>]
 */
class Options {
 public:
  // Lanza std::invalid_argument si falta una opción obligatoria, si una opción
  // no tiene valor, está repetida o no existe, o si -trace no es "y" o "n"
  Options(int argc, char* argv[]);

  const std::string& config_file() const { return config_file_; }
  bool trace() const { return trace_; }
  // Vacío si no se indicó -in: las cadenas se leen de teclado
  const std::string& input_file() const { return input_file_; }
  // Vacío si no se indicó -out: la traza se muestra por pantalla
  const std::string& output_file() const { return output_file_; }

  // Texto de ayuda con la forma de invocar el programa
  static std::string Usage(const std::string& program);

 private:
  std::string config_file_;
  bool trace_ = false;
  std::string input_file_;
  std::string output_file_;
};

#endif  // OPTIONS_H_
