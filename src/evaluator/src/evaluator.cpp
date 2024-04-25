#include "evaluator.hpp"

Object *Evaluator::eval_program(std::vector<Statement *> statements,
                                Environment *env)
{
  Object *result;

  for (auto statement : statements)
  {
    switch (statement->type)
    {
    case LETSTATEMENT:
      return eval(statement, env);

    case RETURNSTATEMENT:
    {
      result = new Object;
      auto value = eval(((ReturnStatement *)statement)->val, env);
      result->type = RETURNVAL;
      result->returnLiteral = new ReturnLiteral{value};
      return result;
    }

    case EXPRSTATEMENT:
    {
      result = eval(((ExpressionStatement *)statement)->exp, env);
      break;
    }
    }

    if (result->type != UNINTITIALIZED)
    {
      if (result->type == ERROR || result->type == RETURNVAL)
      {
        return result;
      }
    }
  }
  return result;
}

Object *Evaluator::eval_prefix_expression(AstNode *node, Object *right)
{
  auto op = ((PrefixExpression *)(node))->op;

  if (op == "!")
  {
    return eval_boolean_operator(right);
  }
  else if (op == "-")
  {
    return eval_minus_operator(right);
  }
  else
  {
    int value = right->integerLiteral->value;
    std::string msg = "Unknown operator: " + op + " " + std::to_string(value);
    Object *result;
    result = new Object;
    result->type = ERROR;
    result->error = new Error{msg};
    return result;
  }
}

Object *Evaluator::eval_infix_expression(AstNode *node, Object *left,
                                         Object *right)
{

  if (left->type == INTEGER && right->type == INTEGER)
  {
    return eval_integer_infix_operator(node, left, right);
  }
  else if (left->type == BOOLEAN && right->type == BOOLEAN)
  {
    return eval_boolean_infix_operator(node, left, right);
  }
  else if (left->type != right->type)
  {
    Object *result;
    auto leftType = left->get_type();
    auto rightType = right->get_type();
    std::string msg = "Type Mismatch: " + leftType + " " + rightType;
    result = new Object;
    result->type = ERROR;
    result->error = new Error{msg};
    return result;
  }
}

Object *Evaluator::eval_integer_infix_operator(AstNode *node, Object *left,
                                               Object *right)
{
  Object *result = new Object;
  auto op = ((InfixExpression *)(node))->op;
  int leftValue = left->integerLiteral->value;
  int rightValue = right->integerLiteral->value;

  if (op == "+")
  {
    int value = (leftValue + rightValue);
    ((Integer *)node)->value = value;
    result->type = INTEGER;
    result->integerLiteral =
        new IntegerLiteral{((Integer *)node)->value = (leftValue + rightValue)};
  }
  else if (op == "-")
  {
    int value = (leftValue - rightValue);
    ((Integer *)node)->value = value;
    result->type = INTEGER;
    result->integerLiteral =
        new IntegerLiteral{value};
  }
  else if (op == "*")
  {
    int value = (leftValue * rightValue);
    ((Integer *)node)->value = value;
    result->type = INTEGER;
    result->integerLiteral =
        new IntegerLiteral{value};
  }
  else if (op == "/")
  {
    int value = (leftValue / rightValue);
    ((Integer *)node)->value = value;
    result->type = INTEGER;
    result->integerLiteral =
        new IntegerLiteral{((Integer *)node)->value = (leftValue / rightValue)};
  }
  else if (op == "<")
  {
    bool value = (leftValue < rightValue);
    ((Boolean *)node)->value = value;
    result->type = BOOLEAN;
    result->booleanLiteral =
        new BooleanLiteral{value};
  }
  else if (op == ">")
  {
    bool value = (leftValue > rightValue);
    ((Boolean *)node)->value = value;
    result->type = BOOLEAN;
    result->booleanLiteral =
        new BooleanLiteral{value};
  }
  else if (op == "==")
  {
    bool value = (leftValue == rightValue);
    ((Boolean *)node)->value = value;
    result->type = BOOLEAN;
    result->booleanLiteral =
        new BooleanLiteral{value};
  }
  else if (op == "!=")
  {
    bool value = (leftValue != rightValue);
    ((Boolean *)node)->value = value;
    result->type = BOOLEAN;
    result->booleanLiteral =
        new BooleanLiteral{value};
  }
  else
  {
    std::string msg = "Unknown operator: " + op + "\n";
    result->type = ERROR;
    result->error = new Error{msg};
  }
  return result;
}

Object *Evaluator::eval_boolean_infix_operator(AstNode *node, Object *left,
                                               Object *right)
{
  Object *result = new Object;
  auto op = ((InfixExpression *)node)->op;

  int leftValue = left->booleanLiteral->value;
  int rightValue = right->booleanLiteral->value;

  if (op == "==")
  {
    bool value = (leftValue == rightValue);
    ((Boolean *)node)->value = value;
    result->type = BOOLEAN;
    result->booleanLiteral = new BooleanLiteral{value};
    return result;
  }
  else if (op == "!=")
  {
    bool value = (leftValue != rightValue);
    ((Boolean *)node)->value = value;
    result->type = BOOLEAN;
    result->booleanLiteral = new BooleanLiteral{value};
    return result;
  }
  else
  {
    std::string msg = "Unknown operator: " + op + "\n";
    result->type = ERROR;
    result->error = new Error{msg};
    return result;
  }
}

Object *Evaluator ::eval_boolean_operator(Object *right)
{
  Object *result = new Object;
  result->type = BOOLEAN;

  switch (right->integerLiteral->value)
  {
  case 0:
  {
    result->booleanLiteral = new BooleanLiteral{true};
    return result;
  }

  default:
  {
    result->booleanLiteral = new BooleanLiteral{false};
    return result;
  }
  }
}

Object *Evaluator ::eval_minus_operator(Object *right)
{
  Object *result = new Object;
  if (right->type != INTEGER)
  {
    std::string msg = "Unknown operator: -" + right->get_type() + "\n";
    result->type = ERROR;
    result->error = new Error{msg};
    return result;
  }
  int value = right->integerLiteral->value;
  result->type = INTEGER;
  result->integerLiteral = new IntegerLiteral{-value};
  return result;
}

std::vector<Object *>
Evaluator ::eval_expressions(std::vector<Expression *> expressions,
                             Environment *env)
{
  std::vector<Object *> result;
  for (auto &expression : expressions)
  {
    auto evaluated = eval(expression, env);
    if (is_error(evaluated))
      return std::vector<Object *>{evaluated};
    result.push_back(evaluated);
  }
  return result;
}

Object *Evaluator::eval(AstNode *node, Environment *env)
{
  switch (node->nodeType)
  {
  case PROGRAM:
  {
    auto programNode = (Program *)node;
    return eval_program(programNode->statements, env);
  }

  case EXPRESSION:
  {
    Expression *exprNode = (Expression *)node;
    switch (exprNode->expressionType)
    {

    case IDENTIFIEREXPR:
    {
      auto identifier = ((Identifier *)node);
      auto id = ((Identifier *)node)->id;
      return env->get(id);
    }

    case BOOLEXPR:
    {
      Object *result = new Object;
      bool value = ((Boolean *)exprNode)->value;
      result->type = BOOLEAN;
      result->booleanLiteral = new BooleanLiteral{value};
      return result;
    }

    case INTEGEREXPR:
    {
      Object *result = new Object;
      int value = ((Integer *)exprNode)->value;
      result->type = INTEGER;
      result->integerLiteral = new IntegerLiteral{value};
      return result;
    }

    case PREFIXEXPR:
    {
      auto expr = (((Expression *)(node))->right);
      auto right = eval(expr, env);
      if (is_error(right))
        return right;
      return eval_prefix_expression(node, right);
    }

    case INFIXEXPR:
    {
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

    case IFEXPR:
    {
      auto expr = (IfExpression *)node;
      auto condition = eval(expr->cond, env);

      if (is_error(condition))
        return condition;

      if (is_truthy(condition))
      {
        return eval(expr->conseq, env);
      }
      else if (expr->altern != NULL)
      {
        return eval(expr->altern, env);
      }
      else
      {
        Object *result = new Object;
        result->type = NULLVAL;
        result->nullLiteral = new NullLiteral();
        return result;
      }
    }

    case FNEXPR:
    {
      auto expr = (FunctionLiteral *)node;
      auto body = expr->body;
      auto parameters = expr->parameters;
      Object *result = new Object;
      result->type = FUNCTIONOBJ;
      result->function = new Function;
      result->function->body = body;
      result->function->parameters = parameters;
      return result;
    }

    case CALLEXPR:
    {
      auto callExpression = (CallExpression *)node;
      auto function = eval(callExpression->function, env);
      if (is_error(function))
        return function;
      auto arguements = eval_expressions(callExpression->arguements, env);
      return apply_function(function, arguements);
    }
    }
  }

  case STATEMENT:
  {
    auto statementNode = (Statement *)node;

    switch (statementNode->type)
    {

    case LETSTATEMENT:
    {
      auto identifierExpression =
          (Identifier *)((LetStatement *)statementNode)->id;
      auto id = identifierExpression->id;
      auto valueExpression = ((LetStatement *)statementNode)->val;
      auto value = eval(valueExpression, env);
      if (is_error(value))
        return value;
      return env->set(id, value);
    }

    case BLOCKSTATEMENT:
    {
      auto statements = ((BlockStatement *)statementNode)->statements;
      return eval_block_statements(statements, env);
    }

    case RETURNSTATEMENT:
    {
      auto valueExpression = ((ReturnStatement *)statementNode)->val;
      auto value = eval(valueExpression, env);
      if (is_error(value))
        return value;

      Object *result = new Object;
      result->type = RETURNVAL;
      result->returnLiteral = new ReturnLiteral{value};
      return result;
    }
    }
  }
  }
}

Object *Evaluator::eval_block_statements(std::vector<Statement *> statements,
                                         Environment *env)
{
  Object *result;
  for (auto statement : statements)
  {
    auto statementNode = (AstNode *)statement;
    switch (statement->type)
    {
    case LETSTATEMENT:
    {
      return eval(statement, env);
    }

    case RETURNSTATEMENT:
    {
      auto value = eval(((ReturnStatement *)statementNode)->val, env);
      result = new Object;
      result->type = RETURNVAL;
      result->returnLiteral = new ReturnLiteral{value};
      return result;
    }

    case EXPRSTATEMENT:
    {
      result = eval(((ExpressionStatement *)statementNode)->exp, env);
      break;
    }
    }

    if (result->type != UNINTITIALIZED)
    {
      if (result->type == ERROR || result->type == RETURNVAL)
        return result;
    }
  }
  return result;
}

Object *Evaluator ::apply_function(Object *object,
                                   std::vector<Object *> arguements)
{

  Environment *extendedEnv = extend_function_env(object, arguements);

  auto body = object->function->body;
  auto parameters = object->function->parameters;

  for (int i = 0; i < parameters.size(); i++)
    extendedEnv->set(parameters[i]->id, arguements[i]);

  auto evaluated = eval(body, extendedEnv);
  return unwrap_return_value(evaluated);
}

Environment *Evaluator ::extend_function_env(Object *object,
                                             std::vector<Object *> arguements)
{
  Environment *env = new Environment(object->function->env);
  return env;
}

Object *Evaluator::unwrap_return_value(Object *result)
{
  if (result->type == RETURNVAL)
  {
    return result->returnLiteral->value;
  }
  return result;
}

bool Evaluator ::is_truthy(Object *condition)
{
  if (condition->type == BOOLEAN)
  {
    return condition->booleanLiteral->value;
  }
  else if (condition->type == NULLVAL)
  {
    return false;
  }
  else
  {
    return true;
  }
}