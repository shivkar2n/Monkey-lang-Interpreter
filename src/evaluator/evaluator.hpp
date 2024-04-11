#include "../parser/ast.hpp"
#include "environment.hpp"
#include "object.hpp"
#include <iostream>

class Evaluator {
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

  Environment *extend_function_env(Object *function,
                                   std::vector<Object *> arguements);
  Object *unwrap_return_value(Object *object);
  bool is_truthy(Object *condition);
};

Object *Evaluator::eval_program(std::vector<Statement *> statements,
                                Environment *env) {
  Object *result;
  for (auto statement : statements) {
    switch (statement->type) {
    case LETSTATEMENT:
      return eval(statement, env);

    case RETURNSTATEMENT: {
      auto value = eval(((ReturnStatement *)statement)->val, env);
      return (Object *)new ReturnLiteral(value);
    }

    case EXPRSTATEMENT: {
      result = eval(((ExpressionStatement *)statement)->exp, env);
      break;
    }
    }

    if (result != NULL) {
      if (result->type == ERROR || result->type == RETURNVAL) {
        return result;
      }
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

  } else {
    int value = (((IntegerLiteral *)(right))->value);
    std::string msg = "Unknown operator: " + op + " " + std::to_string(value);
    return (Object *)new Error(msg);
  }
}

Object *Evaluator::eval_infix_expression(AstNode *node, Object *left,
                                         Object *right) {

  if (left->type == INTEGER && right->type == INTEGER) {
    return eval_integer_infix_operator(node, left, right);

  } else if (left->type == BOOLEAN && right->type == BOOLEAN) {
    return eval_boolean_infix_operator(node, left, right);

  } else if (left->type != right->type) {
    auto leftType = left->get_type();
    auto rightType = right->get_type();
    std::string msg = "Type Mismatch: " + leftType + " " + rightType;
    return (Object *)new Error(msg);
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
    std::string msg = "Unknown operator: " + op + "\n";
    return (Object *)new Error(msg);
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
  } else {
    std::string msg = "Unknown operator: " + op + "\n";
    return (Object *)new Error(msg);
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
  if (right->type != INTEGER) {
    std::string msg = "Unknown operator: -" + right->get_type() + "\n";
    return (Object *)new Error(msg);
  }
  int value = (((IntegerLiteral *)(right))->value);
  return (Object *)new IntegerLiteral(-value);
}

std::vector<Object *>
Evaluator ::eval_expressions(std::vector<Expression *> expressions,
                             Environment *env) {
  std::vector<Object *> result;
  for (auto &expression : expressions) {
    auto evaluated = eval(expression, env);
    if (is_error(evaluated))
      return std::vector<Object *>{evaluated};
    result.push_back(evaluated);
  }
  return result;
}

Object *Evaluator::eval(AstNode *node, Environment *env) {
  switch (node->nodeType) {
  case PROGRAM: {
    auto programNode = (Program *)node;
    return eval_program(programNode->statements, env);
  }

  case EXPRESSION: {
    Expression *exprNode = (Expression *)node;
    switch (exprNode->expressionType) {

    case IDENTIFIEREXPR: {
      auto identifier = ((Identifier *)node);
      auto id = ((Identifier *)node)->id;
      return env->get(id);
    }

    case BOOLEXPR: {
      return (Object *)new BooleanLiteral(((Boolean *)exprNode)->value);
    }

    case INTEGEREXPR: {
      return (Object *)new IntegerLiteral(((Integer *)exprNode)->value);
    }

    case PREFIXEXPR: {
      auto expr = (((Expression *)(node))->right);
      auto right = eval(expr, env);
      if (is_error(right))
        return right;
      return eval_prefix_expression(node, right);
    }

    case INFIXEXPR: {
      auto expr = (((Expression *)(node))->left);
      auto left = eval(expr, env);
      if (is_error(left))
        return left;

      expr = (((Expression *)(node))->right);
      auto right = eval(expr, env);
      if (is_error(right))
        return right;

      return eval_infix_expression(node, left, right);
    }

    case IFEXPR: {
      auto expr = (IfExpression *)node;
      auto condition = eval(expr->cond, env);

      if (is_error(condition))
        return condition;

      if (is_truthy(condition)) {
        return eval(expr->conseq, env);

      } else if (expr->altern != NULL) {
        return eval(expr->altern, env);

      } else {
        return (Object *)(new NullLiteral());
      }
    }

    case FNEXPR: {
      auto expr = (FunctionLiteral *)node;
      auto body = expr->body;
      auto parameters = expr->parameters;
      return (Object *)(new Function(parameters, body, env));
    }

    case CALLEXPR: {
      auto callExpression = (CallExpression *)node;
      auto function = eval(callExpression->function, env);
      if (is_error(function))
        return function;
      auto arguements = eval_expressions(callExpression->arguements, env);
      return apply_function(function, arguements);
    }
    }
  }

  case STATEMENT: {
    auto statementNode = (Statement *)node;

    switch (statementNode->type) {

    case LETSTATEMENT: {
      auto identifierExpression =
          (Identifier *)((LetStatement *)statementNode)->id;
      auto id = identifierExpression->id;
      auto valueExpression = ((LetStatement *)statementNode)->val;
      auto value = eval(valueExpression, env);
      if (is_error(value))
        return value;
      return env->set(id, value);
    }

    case BLOCKSTATEMENT: {
      auto statements = ((BlockStatement *)statementNode)->statements;
      return eval_block_statements(statements, env);
    }

    case RETURNSTATEMENT: {
      auto valueExpression = ((ReturnStatement *)statementNode)->val;
      auto value = eval(valueExpression, env);
      if (is_error(value))
        return value;

      return (Object *)new ReturnLiteral(value);
    }
    }
  }
  }
}

Object *Evaluator::eval_block_statements(std::vector<Statement *> statements,
                                         Environment *env) {
  Object *result = NULL;
  for (auto statement : statements) {
    auto statementNode = (AstNode *)statement;
    switch (statement->type) {
    case LETSTATEMENT: {
      return eval(statement, env);
    }

    case RETURNSTATEMENT: {
      auto value = eval(((ReturnStatement *)statementNode)->val, env);
      return (Object *)new ReturnLiteral(value);
    }

    case EXPRSTATEMENT: {
      result = eval(((ExpressionStatement *)statementNode)->exp, env);
      break;
    }
    }

    if (result != NULL) {
      if (result->type == ERROR || result->type == RETURNVAL) {
        return result;
      }
    }
  }
  return result;
}

Object *Evaluator ::apply_function(Object *function,
                                   std::vector<Object *> arguements) {

  Environment *extendedEnv = extend_function_env(function, arguements);

  auto body = ((Function *)function)->body;
  auto parameters = ((Function *)function)->parameters;

  for (int i = 0; i < parameters.size(); i++)
    extendedEnv->set(parameters[i]->id, arguements[i]);

  auto evaluated = eval(body, extendedEnv);
  return unwrap_return_value(evaluated);
}

Environment *Evaluator ::extend_function_env(Object *function,
                                             std::vector<Object *> arguements) {
  auto functionObj = (Function *)function;
  Environment *env = new Environment(functionObj->env);
  return env;
}

Object *Evaluator::unwrap_return_value(Object *object) {
  if (object->type == RETURNVAL) {
    return ((ReturnLiteral *)object)->value;
  }
  return object;
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