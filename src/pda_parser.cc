/**
 * Complejidad Computacional - Práctica 1
 * Simulador de un autómata con pila por vaciado de pila (APv).
 *
 * pda_parser.cc: implementación de la clase PdaParser.
 */

#include "pda_parser.h"

#include <sstream>
#include <stdexcept>

namespace {

const char kComment = '#';

}  // namespace

PdaParser::PdaParser(std::istream& input) : input_(input) {}

Pda PdaParser::Parse() {
  const std::vector<std::string> state_tokens =
      RequireLine("el conjunto de estados Q");
  const std::set<std::string> states(state_tokens.begin(), state_tokens.end());
  const std::set<char> input_alphabet =
      ToSymbols(RequireLine("el alfabeto de entrada Sigma"));
  const std::set<char> stack_alphabet =
      ToSymbols(RequireLine("el alfabeto de la pila Gamma"));
  const std::string initial_state = RequireSingle("el estado inicial");
  const char initial_stack_symbol =
      ToSymbol(RequireSingle("el simbolo inicial de la pila"));

  std::vector<Transition> transitions;
  std::vector<std::string> tokens;
  while (NextLine(tokens)) {
    if (tokens.size() != 5) {
      Fail("una transicion debe tener 5 elementos: q1 a A1 q2 A");
    }
    transitions.emplace_back(tokens[0], ToSymbol(tokens[1]),
                             ToSymbol(tokens[2]), tokens[3], tokens[4]);
  }

  return Pda(states, input_alphabet, stack_alphabet, initial_state,
             initial_stack_symbol, transitions);
}

bool PdaParser::NextLine(std::vector<std::string>& tokens) {
  std::string line;
  while (std::getline(input_, line)) {
    ++line_number_;
    line = line.substr(0, line.find(kComment));
    std::istringstream words(line);
    tokens.clear();
    std::string word;
    while (words >> word) tokens.push_back(word);
    if (!tokens.empty()) return true;
  }
  return false;
}

std::vector<std::string> PdaParser::RequireLine(const std::string& what) {
  std::vector<std::string> tokens;
  if (!NextLine(tokens)) Fail("falta " + what);
  return tokens;
}

std::string PdaParser::RequireSingle(const std::string& what) {
  const std::vector<std::string> tokens = RequireLine(what);
  if (tokens.size() != 1) Fail("se esperaba un unico elemento para " + what);
  return tokens.front();
}

std::set<char> PdaParser::ToSymbols(
    const std::vector<std::string>& tokens) const {
  std::set<char> symbols;
  for (const std::string& token : tokens) symbols.insert(ToSymbol(token));
  return symbols;
}

char PdaParser::ToSymbol(const std::string& token) const {
  if (token.size() != 1) {
    Fail("el simbolo '" + token + "' debe ser un unico caracter");
  }
  return token.front();
}

void PdaParser::Fail(const std::string& message) const {
  throw std::runtime_error("linea " + std::to_string(line_number_) + ": " +
                           message);
}
