#include "interpreter.hpp"

void Interpreter::print(Object *result)
{
  switch (result->type)
  {

  case INTEGER:
  {
    std::cout << result->integerLiteral->value << '\n';
    break;
  }

  case BOOLEAN:
  {
    std::cout << result->booleanLiteral->value << '\n';
    break;
  }

  case NULLVAL:
  {
    std::cout << "NULL" << '\n';
    break;
  }

  case RETURNVAL:
  {
    print(result->returnLiteral->value);
    break;
  }

  case ERROR:
  {
    std::cout << result->error->value << '\n';
    break;
  }
  }
}

void Interpreter::run(std::string line)
{
  auto tokens = lexer.parse_tokens(line);

  if (tokens.size() == 0)
  {
    std::string msg = "Lexical Error: Invalid Token!";
    throw msg;
  }
  auto program = parser.parse_program(tokens);
  auto result = evaluator.eval(program, environment);
  print(result);
}