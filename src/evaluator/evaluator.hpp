#include "../parser/ast.hpp"
#include "object.hpp"
#include <iostream>

class Evaluator {
public:
  Object *eval_statements(AstNode *node);
  Object *eval(AstNode *node);
  Object *eval_prefix_expression(AstNode *node, Object *right);
  Object *eval_infix_expression(AstNode *node, Object *left, Object *right);
  Object *eval_integer_operator(AstNode *node, Object *left, Object *right);
  Object *eval_boolean_operator(Object *right);
  Object *eval_minus_operator(Object *right);
};

Object *Evaluator::eval_statements(AstNode *node) {
  auto programNode = (Program *)node;
  Object *result;
  for (auto statement : programNode->statements) {
    auto statementNode = (AstNode *)statement;
    switch (statement->type) {
    case LETSTAT:
      break;

    case RETURNSTAT:
      break;

    case EXPRSTAT:
      result = eval(((ExpressionStatement *)statementNode)->exp);
      break;
    }
  }
  return result;
}

Object *Evaluator::eval_prefix_expression(AstNode *node, Object *right) {
  auto op = ((PrefixExpression *)(node))->op;

  if (op == "!") {
    return eval_boolean_operator(right);

  } else if (op == "-") {
    return eval_minus_operator(right);
  }
}

Object *Evaluator::eval_infix_expression(AstNode *node, Object *left,
                                         Object *right) {

  switch (left->type == INTEGER && right->type == INTEGER) {
  case true:
    return eval_integer_operator(node, left, right);

  case false:
    return NULL;
  }
}

Object *Evaluator::eval_integer_operator(AstNode *node, Object *left,
                                         Object *right) {
  auto op = ((InfixExpression *)(node))->op;
  int leftValue = (((IntegerLiteral *)(left))->value);
  int rightValue = (((IntegerLiteral *)(right))->value);

  if (op == "+") {
    return (Object *)new IntegerLiteral(((Integer *)node)->value =
                                            (leftValue + rightValue));

  } else if (op == "-") {
    return (Object *)new IntegerLiteral(((Integer *)node)->value =
                                            (leftValue - rightValue));

  } else if (op == "*") {
    return (Object *)new IntegerLiteral(((Integer *)node)->value =
                                            (leftValue * rightValue));

  } else if (op == "/") {
    return (Object *)new IntegerLiteral(((Integer *)node)->value =
                                            (leftValue / rightValue));
  }
}

Object *Evaluator ::eval_boolean_operator(Object *right) {
  int value = (((IntegerLiteral *)(right))->value);

  switch (value) {
  case 0:
    return (Object *)new BooleanLiteral(true);

  default:
    return (Object *)new BooleanLiteral(false);
  }
}

Object *Evaluator ::eval_minus_operator(Object *right) {
  int value = (((IntegerLiteral *)(right))->value);
  return (Object *)new IntegerLiteral(-value);
}

Object *Evaluator::eval(AstNode *node) {

  switch (node->nodeType) {
  case PROGRAM:
    return eval_statements(node);

  case EXPRESSION: {
    Expression *exprNode = (Expression *)node;
    switch (exprNode->expressionType) {

    case BOOLEXPR: {
      return (Object *)new BooleanLiteral(((Boolean *)exprNode)->value);
    }

    case INTEGEREXPR: {
      return (Object *)new IntegerLiteral(((Integer *)exprNode)->value);
    }

    case PREFIXEXPR: {
      auto expr = (((Expression *)(node))->right);
      auto right = eval(expr);
      return eval_prefix_expression(node, right);
    }

    case INFIXEXPR: {
      auto expr = (((Expression *)(node))->left);
      auto left = eval(expr);
      expr = (((Expression *)(node))->right);
      auto right = eval(expr);
      return eval_infix_expression(node, left, right);
    }
    }
  }
  }
}