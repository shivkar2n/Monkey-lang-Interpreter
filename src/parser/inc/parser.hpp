#include "ast.hpp"
#include <iostream>

class Parser {
public:
  std::vector<Token> tokens;

  Token get_next_token() {
    Token temp = tokens[0];
    tokens.erase(tokens.begin());
    return temp;
  }
  Token curr_token() { return tokens[0]; }
  Token peek_token() { return tokens[1]; }

  int curr_precedence() { return tokenPrecedence[curr_token().tokenType]; }
  int peek_predence() { return tokenPrecedence[peek_token().tokenType]; }
  std::string curr_tokenInteger() {
    return get_token_string[curr_token().tokenType];
  }

  Statement *parse_statement();
  ReturnStatement *parse_return_statement();
  LetStatement *parse_let_statement();
  ExpressionStatement *parse_expr_statement();
  BlockStatement *parse_block_statement();

  Expression *parse_expression(int Predence);
  InfixExpression *parse_infix_expression(Expression *exp);
  PrefixExpression *parse_prefix_expression();
  Expression *parse_group_expression();
  IfExpression *parse_if_expression();
  FunctionLiteral *parse_function_literal();
  std::vector<Identifier *> parse_function_parameters();

  CallExpression *parse_call_expression(Expression *exp);
  std::vector<Expression *> parseCallArguements();

  Expression *parse_infix_function(Expression *exp);
  Expression *parse_prefix_function(TokenType tokenType);

  Expression *parse_identifier();
  Expression *parse_integer_literal();
  Expression *parse_boolean();

  Program *parse_program(std::vector<Token> tokens);
};
