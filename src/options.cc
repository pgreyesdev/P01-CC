/**
 * Complejidad Computacional - Práctica 1
 * Simulador de un autómata con pila por vaciado de pila (APv).
 *
 * options.cc: implementación de la clase Options.
 */

#include "options.h"

#include <map>
#include <stdexcept>

Options::Options(int argc, char* argv[]) {
  // Cada opción va seguida de su valor
  std::map<std::string, std::string> values = {
      {"-config", ""}, {"-trace", ""}, {"-in", ""}, {"-out", ""}};
  for (int i = 1; i < argc; i += 2) {
    const std::string option = argv[i];
    const auto found = values.find(option);
    if (found == values.end()) {
      throw std::invalid_argument("opcion desconocida '" + option + "'");
    }
    if (i + 1 >= argc) {
      throw std::invalid_argument("falta el valor de la opcion " + option);
    }
    if (!found->second.empty()) {
      throw std::invalid_argument("la opcion " + option + " esta repetida");
    }
    found->second = argv[i + 1];
    if (found->second.empty()) {
      throw std::invalid_argument("el valor de la opcion " + option +
                                  " esta vacio");
    }
  }

  if (values["-config"].empty()) {
    throw std::invalid_argument("falta la opcion obligatoria -config");
  }
  const std::string& trace = values["-trace"];
  if (trace.empty()) {
    throw std::invalid_argument("falta la opcion obligatoria -trace");
  }
  if (trace != "y" && trace != "n") {
    throw std::invalid_argument("el valor de -trace debe ser 'y' o 'n'");
  }

  config_file_ = values["-config"];
  trace_ = trace == "y";
  input_file_ = values["-in"];
  output_file_ = values["-out"];
}

std::string Options::Usage(const std::string& program) {
  return "Uso: " + program +
         " -config <f> -trace <y|n> [-in <f>] [-out <f>]\n"
         "  -config <f>   fichero con la definicion del automata\n"
         "  -trace <y|n>  muestra o no la traza\n"
         "  -in <f>       fichero con las cadenas, una por linea "
         "(por defecto, teclado)\n"
         "  -out <f>      fichero donde se escribe la traza "
         "(por defecto, pantalla)\n";
}
