#include "../lexer/lexer.hpp"

enum precedence { LOWEST, EQUALS, LESSGREATER, SUM, PRODUCT, PREFIX, CALL };

std::unordered_map<Tokentype, precedence> tokenPrecedence = {
    {EQUAL, EQUALS},   {NOT_EQUAL, EQUALS}, {GT, LESSGREATER},
    {LT, LESSGREATER}, {PLUS, SUM},         {MINUS, SUM},
    {DIVIDE, PRODUCT}, {MULTIPLY, PRODUCT}, {LPAREN, CALL}};

enum expressionType { ID, LITERAL, PRE, IN, BOOL, IFEXPR, FN, CALLEXP };

class Expression;
class Statement;
class BlockStatement;
class Identifier;

// Definition for expression AST Node
class Expression {
public:
  expressionType type;
  Tokentype tokenType;
  std::string op;
  std::string id;
  int literal;
  Expression *function, *cond, *left, *right;
  BlockStatement *body, *conseq, *altern;
  std::vector<Identifier *> parameters;
  std::vector<Expression *> arguments;

  Expression(expressionType type, Tokentype tokenType, std::string op,
             std::string id, int literal, BlockStatement *body,
             BlockStatement *conseq, BlockStatement *altern,
             Expression *function, Expression *cond, Expression *left,
             Expression *right) {
    this->type = type;
    this->tokenType = tokenType;
    this->op = op;
    this->id = id;
    this->literal = literal;
    this->cond = cond;
    this->conseq = conseq;
    this->altern = altern;
    this->function = function;
    this->left = left;
    this->right = right;
  }
};

class PrefixExpression : public Expression {
public:
  PrefixExpression(Tokentype tokenType, std::string op, Expression *right)
      : Expression(PRE, tokenType, op, "", 0, NULL, NULL, NULL, NULL, NULL,
                   NULL, right) {}
};

class InfixExpression : public Expression {
public:
  InfixExpression(Tokentype tokenType, std::string op, Expression *left,
                  Expression *right)
      : Expression(IN, tokenType, op, "", 0, NULL, NULL, NULL, NULL, NULL, left,
                   right) {}
};

class Identifier : public Expression {
public:
  Identifier(Tokentype tokenType, std::string id)
      : Expression(ID, tokenType, "", id, 0, NULL, NULL, NULL, NULL, NULL, NULL,
                   NULL) {}
};

class Literal : public Expression {
public:
  Literal(Tokentype tokenType, int literal)
      : Expression(LITERAL, tokenType, "", "", literal, NULL, NULL, NULL, NULL,
                   NULL, NULL, NULL) {}
};

class Boolean : public Expression {
public:
  Boolean(Tokentype tokenType, int val)
      : Expression(BOOL, tokenType, "", "", val, NULL, NULL, NULL, NULL, NULL,
                   NULL, NULL) {}
};

class IfExpression : public Expression {
public:
  IfExpression(Tokentype tokenType, BlockStatement *conseq,
               BlockStatement *altern, Expression *cond)
      : Expression(IFEXPR, tokenType, "", "", 0, NULL, conseq, altern, NULL,
                   cond, NULL, NULL) {}
};

class FunctionLiteral : public Expression {
public:
  FunctionLiteral(Tokentype tokenType)
      : Expression(FN, FUNCTION, "", "", 0, NULL, NULL, NULL, NULL, NULL, NULL,
                   NULL) {}
};

class CallExpression : public Expression {
public:
  CallExpression(Tokentype tokenType, Expression *function)
      : Expression(CALLEXP, LPAREN, "", "", 0, NULL, NULL, NULL, function, NULL,
                   NULL, NULL) {}
};

class Statement {
public:
  std::string type;
  Expression *id, *val, *expr;

  Statement(std::string type, Expression *id, Expression *val,
            Expression *expr) {
    this->type = type;
    this->id = id;
    this->val = val;
    this->expr = expr;
  }
};

class LetStatement : public Statement {
public:
  LetStatement(Expression *id, Expression *val)
      : Statement("LET", id, val, NULL) {}
};

class ReturnStatement : public Statement {
public:
  ReturnStatement(Expression *val) : Statement("RETURN", NULL, val, NULL) {}
};

class ExpressionStatement : public Statement {
public:
  ExpressionStatement(Expression *val, Expression *exp)
      : Statement("EXP", NULL, val, exp) {}
};

class BlockStatement {
public:
  Tokentype tokenType;
  std::vector<Statement *> statements;

  BlockStatement(Tokentype tokenType) { this->tokenType = tokenType; }
};