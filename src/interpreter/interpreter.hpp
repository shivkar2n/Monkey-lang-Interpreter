#include "../evaluator/evaluator.hpp"
#include "../parser/parser.hpp"
#include <iostream>

class Interpreter {
  Lexer lexer;
  Parser parser;
  Evaluator evaluator;
  Environment *environment;

public:
  void print(Object *object);
  void run(std::string line);
  Interpreter() { environment = new Environment; }
};

void Interpreter::print(Object *object) {
  switch (object->type) {

  case INTEGER: {
    auto result = (IntegerLiteral *)object;
    std::cout << result->value << '\n';
    break;
  }

  case BOOLEAN: {
    auto result = (BooleanLiteral *)object;
    std::cout << result->value << '\n';
    break;
  }

  case NULLVAL: {
    auto result = (NullLiteral *)object;
    std::cout << "NULL" << '\n';
    break;
  }

  case RETURNVAL: {
    auto result = (ReturnLiteral *)object;
    print(result->value);
    break;
  }

  case ERROR: {
    auto result = (Error *)object;
    std::cout << result->value << '\n';
    break;
  }
  }
}

void Interpreter::run(std::string line) {
  auto tokens = lexer.parse_tokens(line);

  if (tokens.size() == 0) {
    std::string msg = "Lexical Error: Invalid Token!";
    throw msg;
  }
  auto program = parser.parse_program(tokens);
  auto result = evaluator.eval(program, environment);
  print(result);
}