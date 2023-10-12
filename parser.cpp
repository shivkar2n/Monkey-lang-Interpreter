#include "lexer.hpp"
using namespace std;

enum precedence { LOWEST, EQUALS, LESSGREATER, SUM, PRODUCT, PREFIX, CALL };

unordered_map<tokentype, precedence> tokenPrecedence = {
    {EQUAL, EQUALS},   {NOT_EQUAL, EQUALS}, {GT, LESSGREATER},
    {LT, LESSGREATER}, {PLUS, SUM},         {MINUS, SUM},
    {DIVIDE, PRODUCT}, {MULTIPLY, PRODUCT}};

enum expressionType { ID, LITERAL, PRE, IN, BOOL, IFEXPR };

class Expression;
class Statement;
class BlockStatement;

class Expression {
public:
  expressionType type;
  tokentype tokenType;
  string op;   // If node is INFIX
  string id;   // If node is IDENT
  int literal; // If node is INT
  Expression *cond, *left, *right;
  BlockStatement *conseq, *altern;

  Expression(expressionType type, tokentype tokenType, string op, string id,
             int literal, BlockStatement *conseq, BlockStatement *altern,
             Expression *cond, Expression *left, Expression *right) {
    this->type = type;
    this->tokenType = tokenType;
    this->op = op;
    this->id = id;
    this->literal = literal;
    this->cond = cond;
    this->conseq = conseq;
    this->altern = altern;
    this->left = left;
    this->right = right;
  }
};

class PrefixExpression : public Expression {
public:
  PrefixExpression(tokentype tokenType, string op, Expression *right)
      : Expression(PRE, tokenType, op, "", 0, NULL, NULL, NULL, NULL, right) {}
};

class InfixExpression : public Expression {
public:
  InfixExpression(tokentype tokenType, string op, Expression *left,
                  Expression *right)
      : Expression(IN, tokenType, op, "", 0, NULL, NULL, NULL, left, right) {}
};

class Identifier : public Expression {
public:
  Identifier(tokentype tokenType, string id)
      : Expression(ID, tokenType, "", id, 0, NULL, NULL, NULL, NULL, NULL) {}
};

class Literal : public Expression {
public:
  Literal(tokentype tokenType, int literal)
      : Expression(LITERAL, tokenType, "", "", literal, NULL, NULL, NULL, NULL,
                   NULL) {}
};

class Boolean : public Expression {
public:
  Boolean(tokentype tokenType, int val)
      : Expression(BOOL, tokenType, "", "", val, NULL, NULL, NULL, NULL, NULL) {
  }
};

class IfExpression : public Expression {
public:
  IfExpression(tokentype tokenType, BlockStatement *conseq,
               BlockStatement *altern, Expression *cond)
      : Expression(IFEXPR, tokenType, "", "", 0, conseq, altern, cond, NULL,
                   NULL) {}
};

class Statement {
public:
  string type;
  string id;
  string val;
  Expression *expr;

  Statement(string type, string id, string val, Expression *expr) {
    this->type = type;
    this->id = id;
    this->val = val;
    this->expr = expr;
  }
};

class LetStatement : public Statement {
public:
  LetStatement(string id, string val) : Statement("LET", id, val, NULL) {}
};

class ReturnStatement : public Statement {
public:
  ReturnStatement(string val) : Statement("RETURN", "", val, NULL) {}
};

class ExpressionStatement : public Statement {
public:
  ExpressionStatement(string val, Expression *exp)
      : Statement("EXP", "", val, exp) {}
};

class BlockStatement {
public:
  tokentype tokenType;
  vector<Statement *> statements;

  BlockStatement(tokentype tokenType) { this->tokenType = tokenType; }
};

class program {
public:
  vector<Statement *> statements;
  vector<token> tokens;

  token GetNextToken() {
    token temp = tokens[0];
    tokens.erase(tokens.begin());
    return temp;
  }
  token currToken() { return tokens[0]; }
  token peekToken() { return tokens[1]; }

  int currPrecedence() { return tokenPrecedence[currToken().tokenType]; }
  int peekPrecedence() { return tokenPrecedence[peekToken().tokenType]; }
  string currTokenLiteral() { return getTokenStr[currToken().tokenType]; }

  Statement *parseStatement();
  Statement *parseReturnStatement();
  Statement *parseLetStatement();
  Statement *parseExpressionStatement();
  BlockStatement *parseBlockStatement();

  Expression *parseExpression(int precedence);
  Expression *parseInfixExpression(Expression *exp);
  Expression *parsePrefixExpression();
  Expression *parseGroupedExpression();
  Expression *parseIfExpression();

  Expression *parseInfixFn(Expression *exp);
  Expression *parsePrefixFn(tokentype tokenType);

  Expression *parseIdentifer();
  Expression *parseIntegerLiteral();
  Expression *parseBoolean();

  void parseProgram();
};

Statement *program ::parseLetStatement() {
  if (peekToken().tokenType != IDENT) {
    cout << "Parsing Error: Expected IDENT. Got "
         << getTokenStr[peekToken().tokenType];
    return NULL;
  }
  GetNextToken();
  string id = currToken().lexeme;

  if (peekToken().tokenType != ASSIGN) {
    cout << "Parsing Error: Expected ASSIGN. Got "
         << getTokenStr[peekToken().tokenType];
    return NULL;
  }
  GetNextToken();

  if (peekToken().tokenType != INT) {
    cout << "Parsing Error: Expected INT. Got "
         << getTokenStr[peekToken().tokenType];
    return NULL;
  }
  GetNextToken();
  string val = currToken().lexeme;
  // parseExpression();

  if (peekToken().tokenType != SEMICOLON) {
    cout << "Parsing Error: Expected SEMICOLON. Got "
         << getTokenStr[peekToken().tokenType];
    return NULL;
  }
  GetNextToken();

  return (Statement *)(new LetStatement(id, val));
}

Statement *program::parseReturnStatement() {
  if (peekToken().tokenType != INT) {
    cout << "Parsing Error: Expected INT. Got "
         << getTokenStr[peekToken().tokenType];
    return NULL;
  }
  GetNextToken();
  string val = currToken().lexeme;
  // parseExpression();

  if (peekToken().tokenType != SEMICOLON) {
    cout << "Parsing Error: Expected SEMICOLON. Got "
         << getTokenStr[peekToken().tokenType];
    return NULL;
  }
  GetNextToken();

  return (Statement *)(new ReturnStatement(val));
}

Statement *program::parseExpressionStatement() {
  Expression *e = parseExpression(LOWEST);
  if (peekToken().tokenType == SEMICOLON) {
    GetNextToken();
  }
  return (Statement *)(new ExpressionStatement("0", e));
}

Statement *program ::parseStatement() {
  if (currToken().tokenType == LET) {
    return parseLetStatement();
  } else if (currToken().tokenType == RETURN) {
    return parseReturnStatement();
  } else {
    return parseExpressionStatement();
  }
}

void program::parseProgram() {
  while (currToken().tokenType != EOFL) {
    statements.push_back(parseStatement());
    GetNextToken();
  }
}

BlockStatement *program::parseBlockStatement() {
  BlockStatement *block = new BlockStatement(currToken().tokenType);
  GetNextToken();
  while (currToken().tokenType != RBRACE && currToken().tokenType != EOFL) {
    block->statements.push_back(parseStatement());
    GetNextToken();
  }
  return block;
}

Expression *program::parseExpression(int precedence) {
  Expression *leftExp = parsePrefixFn(currToken().tokenType);
  if (leftExp == NULL) {
    cout << "Parsing Error: No prefix function called!!" << endl;
    return NULL;
  }
  while (currToken().tokenType != SEMICOLON && precedence < peekPrecedence()) {
    GetNextToken();
    leftExp = parseInfixFn(leftExp);
  }
  return leftExp;
}

Expression *program::parseInfixExpression(Expression *exp) {
  Expression *node =
      new InfixExpression(currToken().tokenType, currToken().lexeme, exp, NULL);
  GetNextToken();
  node->right = parseExpression(currPrecedence());
  return node;
}

Expression *program::parsePrefixExpression() {
  Expression *node =
      new PrefixExpression(currToken().tokenType, currToken().lexeme, NULL);
  GetNextToken();
  node->right = parseExpression(PREFIX);
  return node;
}

Expression *program::parseGroupedExpression() {
  GetNextToken();
  Expression *exp = parseExpression(LOWEST);
  if (peekToken().tokenType != RPAREN) {
    cout << "Parsing Error: Expected RPAREN. Got "
         << getTokenStr[peekToken().tokenType] << endl;
  }
  GetNextToken();
  return exp;
}

Expression *program::parseIfExpression() {
  Expression *node = new IfExpression(currToken().tokenType, NULL, NULL, NULL);
  if (peekToken().tokenType != LPAREN) {
    cout << "Parsing Error: Expected LPAREN. Got "
         << getTokenStr[peekToken().tokenType] << endl;
  }
  GetNextToken();

  node->cond = parseExpression(LOWEST);
  if (currToken().tokenType != RPAREN) {
    cout << "Parsing Error: Expected RPAREN. Got "
         << getTokenStr[peekToken().tokenType] << endl;
  }
  GetNextToken();

  if (currToken().tokenType != LBRACE) {
    cout << "Parsing Error: Expected LBRACE. Got "
         << getTokenStr[peekToken().tokenType] << endl;
  }

  node->conseq = parseBlockStatement();
  return node;
}

Expression *program::parseInfixFn(Expression *exp) {
  switch (currToken().tokenType) {
  case GT:
    return parseInfixExpression(exp);

  case LT:
    return parseInfixExpression(exp);

  case NOT_EQUAL:
    return parseInfixExpression(exp);

  case EQUAL:
    return parseInfixExpression(exp);

  case PLUS:
    return parseInfixExpression(exp);

  case DIVIDE:
    return parseInfixExpression(exp);

  case MULTIPLY:
    return parseInfixExpression(exp);

  default:
    return NULL;
  }
}

Expression *program::parsePrefixFn(tokentype type) {
  switch (type) {
  case IF:
    return parseIfExpression();

  case IDENT:
    return parseIdentifer();

  case INT:
    return parseIntegerLiteral();

  case MINUS:
    return parsePrefixExpression();

  case BANG:
    return parsePrefixExpression();

  case TRUE:
    return parseBoolean();

  case FALSE:
    return parseBoolean();

  case LPAREN:
    return parseGroupedExpression();

  default:
    return NULL;
  }
}

Expression *program::parseIdentifer() {
  return new Identifier(currToken().tokenType, currToken().lexeme);
}

Expression *program::parseIntegerLiteral() {
  return new Literal(currToken().tokenType, stoi(currToken().lexeme));
}

Expression *program::parseBoolean() {
  return new Boolean(currToken().tokenType, currToken().lexeme == "true");
}

int main(int argc, char *argv[]) {
  string data;
  vector<token> tokens;
  if (parseFile(data) == -1) {
    cout << "I/O Error: Unable to open file!";
    return -1;
  }

  if (parseTokens(data, tokens) == -1) {
    cout << "Lexical Error: Invalid Token!";
    return -1;
  }

  program p;
  p.tokens = tokens;
  p.parseProgram();
  return 0;
}
