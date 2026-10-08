/**
 * Complejidad Computacional - Práctica 1
 * Simulador de un autómata con pila por vaciado de pila (APv).
 *
 * main.cc: punto de entrada del programa.
 */

#include <exception>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

#include "options.h"
#include "pda.h"
#include "pda_parser.h"

namespace {

// Lee el autómata y comprueba las cadenas de entrada. Devuelve el código de
// salida del programa
int Run(const Options& options) {
  std::ifstream config(options.config_file());
  if (!config) {
    std::cerr << "Error: no se puede abrir el fichero '"
              << options.config_file() << "'\n";
    return 1;
  }

  // Las cadenas se leen del fichero de -in o, si no se indicó, de teclado
  std::ifstream input_file;
  std::istream* input = &std::cin;
  if (!options.input_file().empty()) {
    input_file.open(options.input_file());
    if (!input_file) {
      std::cerr << "Error: no se puede abrir el fichero '"
                << options.input_file() << "'\n";
      return 1;
    }
    input = &input_file;
  }

  // La traza se escribe en el fichero de -out o, si no se indicó, por pantalla
  std::ofstream output_file;
  std::ostream* trace = nullptr;
  if (options.trace()) {
    trace = &std::cout;
    if (!options.output_file().empty()) {
      output_file.open(options.output_file());
      if (!output_file) {
        std::cerr << "Error: no se puede crear el fichero '"
                  << options.output_file() << "'\n";
        return 1;
      }
      trace = &output_file;
    }
  }

  try {
    const Pda pda = PdaParser(config).Parse();
    std::cout << pda;

    if (input == &std::cin) {
      std::cout << "Introduzca las cadenas, una por linea (una linea vacia es "
                   "la cadena vacia).\nPara terminar: Ctrl+D en Linux, Ctrl+Z "
                   "y Enter en Windows.\n";
    }
    std::string line;
    while (std::getline(*input, line)) {
      // Ficheros con fin de línea de Windows leídos en Linux
      if (!line.empty() && line.back() == '\r') line.pop_back();
      if (trace != nullptr) *trace << "Cadena '" << line << "':\n";
      std::string verdict;
      try {
        verdict = pda.Accepts(line, trace) ? "pertenece" : "no pertenece";
      } catch (const StepLimitExceeded& error) {
        verdict = error.what();
      }
      const std::string result = "'" + line + "': " + verdict;
      std::cout << result << "\n";
      // El fichero de traza incluye también el resultado de cada cadena
      if (trace == &output_file) *trace << result << "\n";
      if (trace != nullptr) *trace << "\n";
    }
  } catch (const std::exception& error) {
    std::cerr << "Error en '" << options.config_file() << "': " << error.what()
              << "\n";
    return 1;
  }
  return 0;
}

}  // namespace

int main(int argc, char* argv[]) {
  try {
    return Run(Options(argc, argv));
  } catch (const std::invalid_argument& error) {
    std::cerr << "Error: " << error.what() << "\n" << Options::Usage(argv[0]);
    return 1;
  }
}
