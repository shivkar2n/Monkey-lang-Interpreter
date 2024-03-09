#include "../parser/ast.hpp"
#include "object.hpp"
#include <iostream>

class Evaluator {
public:
  Object *eval_statements(std::vector<Statement *> statements);
  Object *eval(AstNode *node);
  Object *eval_prefix_expression(AstNode *node, Object *right);
  Object *eval_infix_expression(AstNode *node, Object *left, Object *right);
  Object *eval_integer_infix_operator(AstNode *node, Object *left,
                                      Object *right);
  Object *eval_boolean_operator(Object *right);
  Object *eval_boolean_infix_operator(AstNode *node, Object *left,
                                      Object *right);
  Object *eval_minus_operator(Object *right);
  bool is_truthy(Object *condition);
};

Object *Evaluator::eval_statements(std::vector<Statement *> statements) {
  Object *result;
  for (auto statement : statements) {
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

  if (left->type == INTEGER && right->type == INTEGER) {
    return eval_integer_infix_operator(node, left, right);

  } else if (left->type == BOOLEAN && right->type == BOOLEAN) {
    return eval_boolean_infix_operator(node, left, right);
  }
}

Object *Evaluator::eval_integer_infix_operator(AstNode *node, Object *left,
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
  } else if (op == "<") {
    return (Object *)new BooleanLiteral(((Boolean *)node)->value =
                                            (leftValue < rightValue));
  } else if (op == ">") {
    return (Object *)new BooleanLiteral(((Boolean *)node)->value =
                                            (leftValue > rightValue));
  } else if (op == "==") {
    return (Object *)new BooleanLiteral(((Boolean *)node)->value =
                                            (leftValue == rightValue));
  } else if (op == "!=") {
    return (Object *)new BooleanLiteral(((Boolean *)node)->value =
                                            (leftValue != rightValue));
  } else {
    return (Object *)new NullLiteral();
  }
}

Object *Evaluator::eval_boolean_infix_operator(AstNode *node, Object *left,
                                               Object *right) {

  auto op = ((InfixExpression *)node)->op;
  int leftValue = (((BooleanLiteral *)left)->value);
  int rightValue = (((BooleanLiteral *)right)->value);

  if (op == "==") {
    return (Object *)new BooleanLiteral(((Boolean *)node)->value =
                                            (leftValue == rightValue));
  } else if (op == "!=") {
    return (Object *)new BooleanLiteral(((Boolean *)node)->value =
                                            (leftValue != rightValue));
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
  case PROGRAM: {
    auto programNode = (Program *)node;
    return eval_statements(programNode->statements);
  }

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

    case IFEXPR: {
      auto expr = (IfExpression *)node;
      auto condition = eval(expr->cond);

      if (is_truthy(condition)) {
        return eval(expr->conseq);

      } else if (expr->altern != NULL) {
        return eval(expr->altern);

      } else {
        return (Object *)(new NullLiteral());
      }
    }
    }
  }

  case STATEMENT: {
    auto statementNode = (Statement *)node;
    switch (statementNode->type) {
    case BLOCKSTAT: {
      auto statements = ((BlockStatement *)statementNode)->statements;
      return eval_statements(statements);
    }
    }
  }
  }
}

bool Evaluator ::is_truthy(Object *condition) {
  if (condition->type == BOOLEAN) {
    auto boolean = (BooleanLiteral *)condition;
    return boolean->value;

  } else if (condition->type == NULLVAL) {
    return false;

  } else {
    return true;
  }
}