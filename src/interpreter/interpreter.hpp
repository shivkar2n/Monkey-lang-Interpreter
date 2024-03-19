#include "../evaluator/evaluator.hpp"
#include "../parser/parser.hpp"
#include <iostream>

class Interpreter {
  Lexer lexer;
  Parser parser;
  Evaluator evaluator;

public:
  void run(std::string line) {
    auto tokens = lexer.parse_tokens(line);

    if (tokens.size() == 0) {
      std::string msg = "Lexical Error: Invalid Token!";
      throw msg;
    }

    auto program = parser.parse_program(tokens);
    auto result = (ReturnLiteral *)evaluator.eval(program);

    std::cout << ((IntegerLiteral *)(result->value))->value << '\n';
  }
};