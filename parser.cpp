#include "lexer.hpp"
using namespace std;

// Precendences
enum precedence { LOWEST, EQUALS, LESSGREATER, SUM, PRODUCT, PREFIX, CALL };

// Map token to given predence
unordered_map<tokentype, precedence> tokenPrecedence = {
    {EQUAL, EQUALS},   {NOT_EQUAL, EQUALS}, {GT, LESSGREATER},
    {LT, LESSGREATER}, {PLUS, SUM},         {MINUS, SUM},
    {DIVIDE, PRODUCT}, {MULTIPLY, PRODUCT}
    //
};

// Expresson can be of type: INFIX | PREFIX | IDENTIFIER | LITERAL | BOOLEAN
enum expressionType { ID, LITERAL, PRE, IN, BOOL };

class Expression { // Node of an Expression Tree
public:
  expressionType type;
  tokentype tokenType;
  string op;   // If node is INFIX
  string id;   // If node is IDENT
  int literal; // If node is INT
  Expression *left;
  Expression *right;

  Expression(expressionType type, tokentype tokenType, string op, string id,
             int literal, Expression *left, Expression *right) {
    this->type = type;
    this->tokenType = tokenType;
    this->op = op;
    this->id = id;
    this->literal = literal;
    this->left = left;
    this->right = right;
  }
};

class PrefixExpression : public Expression {
public:
  PrefixExpression(tokentype tokenType, string op, Expression *right)
      : Expression(PRE, tokenType, op, "", 0, NULL, right) {}
};

class InfixExpression : public Expression {
public:
  InfixExpression(tokentype tokenType, string op, Expression *left,
                  Expression *right)
      : Expression(IN, tokenType, op, "", 0, left, right) {}
};

class Identifier : public Expression {
public:
  Identifier(tokentype tokenType, string id)
      : Expression(ID, tokenType, "", id, 0, NULL, NULL) {}
};

class Literal : public Expression {
public:
  Literal(tokentype tokenType, int literal)
      : Expression(LITERAL, tokenType, "", "", literal, NULL, NULL) {}
};

class Boolean : public Expression {
public:
  Boolean(tokentype tokenType, int val)
      : Expression(BOOL, tokenType, "", "", val, NULL, NULL) {}
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

// Main root node
class program {
public:
  vector<Statement> statements;
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

  int parseStatement();
  int parseReturnStatement();
  int parseLetStatement();
  int parseExpressionStatement();

  Expression *parseExpression(int precedence);
  Expression *parseInfixExpression(Expression *exp);
  Expression *parsePrefixExpression();
  Expression *parseGroupedExpression();
  Expression *parseInfixFn(Expression *exp);
  Expression *parsePrefixFn(tokentype tokenType);

  Expression *parseIdentifer();
  Expression *parseIntegerLiteral();
  Expression *parseBoolean();
};

int program ::parseLetStatement() {
  if (peekToken().tokenType != IDENT) {
    cout << "Parsing Error: Expected IDENT. Got "
         << getTokenStr[peekToken().tokenType];
    return -1;
  }
  GetNextToken();
  string id = currToken().lexeme;

  if (peekToken().tokenType != ASSIGN) {
    cout << "Parsing Error: Expected ASSIGN. Got "
         << getTokenStr[peekToken().tokenType];
    return -1;
  }
  GetNextToken();

  if (peekToken().tokenType != INT) {
    cout << "Parsing Error: Expected INT. Got "
         << getTokenStr[peekToken().tokenType];
    return -1;
  }
  GetNextToken();
  string val = currToken().lexeme;
  // parseExpression();

  if (peekToken().tokenType != SEMICOLON) {
    cout << "Parsing Error: Expected SEMICOLON. Got "
         << getTokenStr[peekToken().tokenType];
    return -1;
  }
  GetNextToken();

  statements.push_back((Statement)LetStatement(id, val));
  return 0;
}

int program::parseReturnStatement() {
  if (peekToken().tokenType != INT) {
    cout << "Parsing Error: Expected INT. Got "
         << getTokenStr[peekToken().tokenType];
    return -1;
  }
  GetNextToken();
  string val = currToken().lexeme;
  // parseExpression();

  if (peekToken().tokenType != SEMICOLON) {
    cout << "Parsing Error: Expected SEMICOLON. Got "
         << getTokenStr[peekToken().tokenType];
    return -1;
  }
  GetNextToken();

  statements.push_back((Statement)ReturnStatement(val));
  return 0;
}

int program::parseExpressionStatement() {
  Expression *e = parseExpression(LOWEST);
  if (peekToken().tokenType == SEMICOLON) {
    GetNextToken();
  }
  Statement s = ExpressionStatement("0", e);
  statements.push_back(s);
  return 0;
}

void printProgram(program &p) {
  for (auto &statement : p.statements) {
    cout << statement.type << endl;
  }
}

int program::parseStatement() {
  while (currToken().tokenType != EOFL) {
    if (currToken().tokenType == LET) {
      if (parseLetStatement() == -1)
        return -1;
    } else if (currToken().tokenType == RETURN) {
      if (parseReturnStatement() == -1)
        return -1;
    } else {
      parseExpressionStatement();
    }
    GetNextToken();
  }
  // printProgram(p);
  return 0;
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
  p.parseStatement();

  return 0;
}
