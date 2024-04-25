#include "ast.hpp"
#include "environment.cpp"
#include <iostream>

class Evaluator
{
public:
  Object *eval_program(std::vector<Statement *> statements, Environment *env);
  Object *eval(AstNode *node, Environment *env);
  Object *eval_prefix_expression(AstNode *node, Object *right);
  Object *eval_infix_expression(AstNode *node, Object *left, Object *right);
  Object *eval_integer_infix_operator(AstNode *node, Object *left,
                                      Object *right);
  Object *eval_boolean_operator(Object *right);
  Object *eval_boolean_infix_operator(AstNode *node, Object *left,
                                      Object *right);
  Object *eval_minus_operator(Object *right);
  Object *eval_block_statements(std::vector<Statement *> statements,
                                Environment *env);
  std::vector<Object *> eval_expressions(std::vector<Expression *> nodes,
                                         Environment *env);

  Object *apply_function(Object *function, std::vector<Object *> arguements);

  Environment *extend_function_env(Object *object,
                                   std::vector<Object *> arguements);
  Object *unwrap_return_value(Object *object);
  bool is_truthy(Object *condition);
};
