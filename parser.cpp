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

int currPrecedence(vector<token> &tokens) {
  return tokenPrecedence[currToken(tokens).tokenType];
}

int peekPrecedence(vector<token> &tokens) {
  return tokenPrecedence[peekToken(tokens).tokenType];
}

string currTokenLiteral(vector<token> &tokens) {
  return getTokenStr[currToken(tokens).tokenType];
}

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

Expression *parseExpression(vector<token> &tokens, int precedence);
Expression *parseInfixExpression(vector<token> &tokens, Expression *exp);
Expression *parsePrefixExpression(vector<token> &tokens);
Expression *parseInfixFn(vector<token> &tokens, Expression *exp);
Expression *parsePrefixFn(vector<token> &tokens, tokentype tokenType);

Expression *parseIdentifer(vector<token> &tokens);
Expression *parseIntegerLiteral(vector<token> &tokens);
Expression *parseBoolean(vector<token> &tokens);

Expression *parseExpression(vector<token> &tokens, int precedence) {
  Expression *leftExp = parsePrefixFn(tokens, currToken(tokens).tokenType);
  if (leftExp == NULL) {
    cout << "Parsing Error: No prefix function called!!" << endl;
    return NULL;
  }
  while (currToken(tokens).tokenType != SEMICOLON &&
         precedence < peekPrecedence(tokens)) {
    GetNextToken(tokens);
    leftExp = parseInfixFn(tokens, leftExp);
  }
  return leftExp;
}

Expression *parseInfixExpression(vector<token> &tokens, Expression *exp) {
  Expression *node = new InfixExpression(currToken(tokens).tokenType,
                                         currToken(tokens).lexeme, exp, NULL);
  GetNextToken(tokens);
  node->right = parseExpression(tokens, currPrecedence(tokens));
  return node;
}

Expression *parsePrefixExpression(vector<token> &tokens) {
  Expression *node = new PrefixExpression(currToken(tokens).tokenType,
                                          currToken(tokens).lexeme, NULL);
  GetNextToken(tokens);
  node->right = parseExpression(tokens, PREFIX);
  return node;
}

Expression *parseGroupedExpression(vector<token> &tokens) {
  GetNextToken(tokens);
  Expression *exp = parseExpression(tokens, LOWEST);
  if (peekToken(tokens).tokenType != RPAREN) {
    cout << "Parsing Error: Expected RPAREN. Got "
         << getTokenStr[peekToken(tokens).tokenType] << endl;
  }
  GetNextToken(tokens);
  return exp;
}

Expression *parseInfixFn(vector<token> &tokens, Expression *exp) {
  switch (currToken(tokens).tokenType) {
  case GT:
    return parseInfixExpression(tokens, exp);

  case LT:
    return parseInfixExpression(tokens, exp);

  case NOT_EQUAL:
    return parseInfixExpression(tokens, exp);

  case EQUAL:
    return parseInfixExpression(tokens, exp);

  case PLUS:
    return parseInfixExpression(tokens, exp);

  case MINUS:
    return parseInfixExpression(tokens, exp);

  case DIVIDE:
    return parseInfixExpression(tokens, exp);

  case MULTIPLY:
    return parseInfixExpression(tokens, exp);

  default:
    return NULL;
  }
}

Expression *parsePrefixFn(vector<token> &tokens, tokentype type) {
  switch (type) {
  case IDENT:
    return parseIdentifer(tokens);

  case INT:
    return parseIntegerLiteral(tokens);

  case MINUS:
    return parsePrefixExpression(tokens);

  case BANG:
    return parsePrefixExpression(tokens);

  case TRUE:
    return parseBoolean(tokens);

  case FALSE:
    return parseBoolean(tokens);

  case LPAREN:
    return parseGroupedExpression(tokens);

  default:
    return NULL;
  }
}

Expression *parseIdentifer(vector<token> &tokens) {
  return new Identifier(currToken(tokens).tokenType, currToken(tokens).lexeme);
}

Expression *parseIntegerLiteral(vector<token> &tokens) {
  return new Literal(currToken(tokens).tokenType,
                     stoi(currToken(tokens).lexeme));
}

Expression *parseBoolean(vector<token> &tokens) {
  return new Boolean(currToken(tokens).tokenType,
                     currToken(tokens).lexeme == "true");
}

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
};

int parseLetStatement(program &p, vector<token> &t) {
  if (peekToken(t).tokenType != IDENT) {
    cout << "Parsing Error: Expected IDENT. Got "
         << getTokenStr[peekToken(t).tokenType];
    return -1;
  }
  GetNextToken(t);
  string id = currToken(t).lexeme;

  if (peekToken(t).tokenType != ASSIGN) {
    cout << "Parsing Error: Expected ASSIGN. Got "
         << getTokenStr[peekToken(t).tokenType];
    return -1;
  }
  GetNextToken(t);

  if (peekToken(t).tokenType != INT) {
    cout << "Parsing Error: Expected INT. Got "
         << getTokenStr[peekToken(t).tokenType];
    return -1;
  }
  GetNextToken(t);
  string val = currToken(t).lexeme;
  // parseExpression();

  if (peekToken(t).tokenType != SEMICOLON) {
    cout << "Parsing Error: Expected SEMICOLON. Got "
         << getTokenStr[peekToken(t).tokenType];
    return -1;
  }
  GetNextToken(t);

  p.statements.push_back((Statement)LetStatement(id, val));
  return 0;
}

int parseReturnStatement(program &p, vector<token> t) {
  if (peekToken(t).tokenType != INT) {
    cout << "Parsing Error: Expected INT. Got "
         << getTokenStr[peekToken(t).tokenType];
    return -1;
  }
  GetNextToken(t);
  string val = currToken(t).lexeme;
  // parseExpression();

  if (peekToken(t).tokenType != SEMICOLON) {
    cout << "Parsing Error: Expected SEMICOLON. Got "
         << getTokenStr[peekToken(t).tokenType];
    return -1;
  }
  GetNextToken(t);

  p.statements.push_back((Statement)ReturnStatement(val));
  return 0;
}

int parseExpressionStatement(program &p, vector<token> &t) {
  Expression *e = parseExpression(t, LOWEST);
  if (peekToken(t).tokenType == SEMICOLON) {
    GetNextToken(t);
  }
  Statement s = ExpressionStatement("0", e);
  p.statements.push_back(s);
  return 0;
}

void printProgram(program &p) {
  for (auto &statement : p.statements) {
    cout << statement.type << endl;
  }
}

int parseStatement(program &p, vector<token> &t) {
  while (currToken(t).tokenType != EOFL) {
    if (currToken(t).tokenType == LET) {
      if (parseLetStatement(p, t) == -1)
        return -1;
    } else if (currToken(t).tokenType == RETURN) {
      if (parseReturnStatement(p, t) == -1)
        return -1;
    } else {
      parseExpressionStatement(p, t);
    }
    GetNextToken(t);
  }
  // printProgram(p);
  return 0;
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
  parseStatement(p, tokens);

  return 0;
}
