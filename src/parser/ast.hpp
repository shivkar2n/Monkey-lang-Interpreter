#include "../lexer/lexer.hpp"
#include <string>

#if !defined(PARSER_AST_HPP)
#define PARSER_AST_HPP

enum NodeType { PROGRAM, EXPRESSION, STATEMENT };

enum Precedence { LOWEST, EQUALS, LESSGREATER, SUM, PRODUCT, PREFIX, CALL };

std::unordered_map<TokenType, Precedence> tokenPrecedence = {
    {EQUAL, EQUALS},   {NOT_EQUAL, EQUALS}, {GT, LESSGREATER},
    {LT, LESSGREATER}, {PLUS, SUM},         {MINUS, SUM},
    {DIVIDE, PRODUCT}, {MULTIPLY, PRODUCT}, {LPAREN, CALL}};

enum ExpressionType {
  IDENTIFIEREXPR,
  INTEGEREXPR,
  PREFIXEXPR,
  INFIXEXPR,
  BOOLEXPR,
  IFEXPR,
  FNEXPR,
  CALLEXPR
};

enum StatementType { LETSTAT, EXPRSTAT, RETURNSTAT, BLOCKSTAT };

class AstNode {
public:
  NodeType nodeType;
};

// Base types of AST nodes
class Program;
class Expression;
class Statement;

// Derived types of AST nodes
class BlockStatement;
class Identifier;

class Program : public AstNode {
public:
  std::vector<Statement *> statements;
  Program() { nodeType = PROGRAM; }
};

class Expression : public AstNode {
public:
  int value;
  Expression *left, *right;
  ExpressionType expressionType;
  TokenType tokenType;

  Expression() { this->nodeType = EXPRESSION; }
};

class PrefixExpression : public Expression {
public:
  std::string op;

  PrefixExpression(TokenType tokenType, std::string op, Expression *right) {
    this->expressionType = PREFIXEXPR;
    this->tokenType = tokenType;
    this->op = op;
    this->right = right;
  }
};

class InfixExpression : public Expression {
public:
  std::string op;

  InfixExpression(TokenType tokenType, std::string op, Expression *left,
                  Expression *right) {
    this->expressionType = INFIXEXPR;
    this->tokenType = tokenType;
    this->op = op;
    this->left = left;
    this->right = right;
  }
};

class Identifier : public Expression {
public:
  std::string id;

  Identifier(TokenType tokenType, std::string id) {
    this->expressionType = IDENTIFIEREXPR;
    this->tokenType = tokenType;
    this->id = id;
  }
};

class Integer : public Expression {
public:
  Integer(TokenType tokenType, int value) {
    this->expressionType = INTEGEREXPR;
    this->tokenType = tokenType;
    this->value = value;
  }
};

class Boolean : public Expression {
public:
  Boolean(TokenType tokenType, int value) {
    this->expressionType = BOOLEXPR;
    this->tokenType = tokenType;
    this->value = value;
  }
};

class IfExpression : public Expression {
public:
  BlockStatement *conseq, *altern;
  Expression *cond;

  IfExpression(TokenType tokenType, BlockStatement *conseq,
               BlockStatement *altern, Expression *cond) {
    this->expressionType = IFEXPR;
    this->tokenType = tokenType;
    this->conseq = conseq;
    this->altern = altern;
    this->cond = cond;
  }
};

class FunctionLiteral : public Expression {
public:
  std::vector<Identifier *> parameters;
  BlockStatement *body;

  FunctionLiteral(TokenType tokenType) {
    this->expressionType = FNEXPR;
    this->tokenType = tokenType;
  }
};

class CallExpression : public Expression {
public:
  Expression *function;
  std::vector<Expression *> arguements;

  CallExpression(TokenType tokenType, Expression *function) {
    this->expressionType = CALLEXPR;
    this->tokenType = LPAREN;
    this->function = function;
  }
};

class Statement : public AstNode {
public:
  StatementType type;

  Statement() { this->nodeType = STATEMENT; }
};

class LetStatement : public Statement {
public:
  Expression *id, *val;

  LetStatement(Expression *id, Expression *val) {
    this->type = LETSTAT;
    this->id = id;
    this->val = val;
  }
};

class ReturnStatement : public Statement {
public:
  Expression *val;

  ReturnStatement(Expression *val) {
    this->type = RETURNSTAT;
    this->val = val;
  }
};

class ExpressionStatement : public Statement {
public:
  Expression *exp;

  ExpressionStatement(Expression *exp) {
    this->type = EXPRSTAT;
    this->exp = exp;
  }
};

class BlockStatement : public Statement {
public:
  TokenType tokenType;
  std::vector<Statement *> statements;

  BlockStatement(TokenType tokenType) {
    this->type = BLOCKSTAT;
    this->tokenType = tokenType;
  }
};

#endif