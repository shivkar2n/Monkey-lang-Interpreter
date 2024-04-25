#include "evaluator.cpp"
#include "parser.cpp"
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
