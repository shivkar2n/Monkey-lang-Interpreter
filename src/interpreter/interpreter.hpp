#include "../evaluator/evaluator.hpp"
#include "../parser/parser.hpp"
#include <iostream>

class Interpreter {
  Lexer lexer;
  Parser parser;
  Evaluator evaluator;

public:
  void run(std::string line) {
    if (line == "") {
      std::cout << "I/O Error: Unable to open file!";
      return;
    }
    auto tokens = lexer.parse_tokens(line);

    if (tokens.size() == 0) {
      std::cout << "Lexical Error: Invalid Token!";
      return;
    }

    auto program = parser.parse_program(tokens);
    auto result = (Integer *)evaluator.eval(program);

    std::cout << result->value << '\n';
  }
};