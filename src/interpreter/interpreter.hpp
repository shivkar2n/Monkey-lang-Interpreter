#include "../evaluator/evaluator.cpp"
#include "../parser/parser.cpp"
#include <iostream>

class Interpreter
{
  Lexer lexer;
  Parser parser;
  Evaluator evaluator;
  Environment *environment;

public:
  void print(Object *result);
  void run(std::string line);
  Interpreter() { environment = new Environment; }
};
