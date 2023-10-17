#include "lexer.hpp"
using namespace std;

enum precedence { LOWEST, EQUALS, LESSGREATER, SUM, PRODUCT, PREFIX, CALL };

unordered_map<tokentype, precedence> tokenPrecedence = {
    {EQUAL, EQUALS},   {NOT_EQUAL, EQUALS}, {GT, LESSGREATER},
    {LT, LESSGREATER}, {PLUS, SUM},         {MINUS, SUM},
    {DIVIDE, PRODUCT}, {MULTIPLY, PRODUCT}, {LPAREN, CALL}};

enum expressionType { ID, LITERAL, PRE, IN, BOOL, IFEXPR, FN, CALLEXP };

class Expression;
class Statement;
class BlockStatement;
class Identifier;

class Expression {
public:
  expressionType type;
  tokentype tokenType;
  string op;
  string id;
  int literal;
  Expression *function, *cond, *left, *right;
  BlockStatement *body, *conseq, *altern;
  vector<Identifier *> parameters;
  vector<Expression *> arguments;

  Expression(expressionType type, tokentype tokenType, string op, string id,
             int literal, BlockStatement *body, BlockStatement *conseq,
             BlockStatement *altern, Expression *function, Expression *cond,
             Expression *left, Expression *right) {
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
  PrefixExpression(tokentype tokenType, string op, Expression *right)
      : Expression(PRE, tokenType, op, "", 0, NULL, NULL, NULL, NULL, NULL,
                   NULL, right) {}
};

class InfixExpression : public Expression {
public:
  InfixExpression(tokentype tokenType, string op, Expression *left,
                  Expression *right)
      : Expression(IN, tokenType, op, "", 0, NULL, NULL, NULL, NULL, NULL, left,
                   right) {}
};

class Identifier : public Expression {
public:
  Identifier(tokentype tokenType, string id)
      : Expression(ID, tokenType, "", id, 0, NULL, NULL, NULL, NULL, NULL, NULL,
                   NULL) {}
};

class Literal : public Expression {
public:
  Literal(tokentype tokenType, int literal)
      : Expression(LITERAL, tokenType, "", "", literal, NULL, NULL, NULL, NULL,
                   NULL, NULL, NULL) {}
};

class Boolean : public Expression {
public:
  Boolean(tokentype tokenType, int val)
      : Expression(BOOL, tokenType, "", "", val, NULL, NULL, NULL, NULL, NULL,
                   NULL, NULL) {}
};

class IfExpression : public Expression {
public:
  IfExpression(tokentype tokenType, BlockStatement *conseq,
               BlockStatement *altern, Expression *cond)
      : Expression(IFEXPR, tokenType, "", "", 0, NULL, conseq, altern, NULL,
                   cond, NULL, NULL) {}
};

class FunctionLiteral : public Expression {
public:
  FunctionLiteral(tokentype tokenType)
      : Expression(FN, FUNCTION, "", "", 0, NULL, NULL, NULL, NULL, NULL, NULL,
                   NULL) {}
};

class CallExpression : public Expression {
public:
  CallExpression(tokentype tokenType, Expression *function)
      : Expression(CALLEXP, LPAREN, "", "", 0, NULL, NULL, NULL, function, NULL,
                   NULL, NULL) {}
};

class Statement {
public:
  string type;
  Expression *id, *val, *expr;

  Statement(string type, Expression *id, Expression *val, Expression *expr) {
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

  string parseFile();
  vector<token> parseTokens(string code);

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
  Expression *parseFunctionLiteral();
  vector<Identifier *> parseFunctionParameters();

  Expression *parseCallExpression(Expression *exp);
  vector<Expression *> parseCallArguements();

  Expression *parseInfixFn(Expression *exp);
  Expression *parsePrefixFn(tokentype tokenType);

  Expression *parseIdentifer();
  Expression *parseIntegerLiteral();
  Expression *parseBoolean();

  void parseProgram(string data);
};

vector<token> program::parseTokens(string code) {
  vector<token> M;
  int i = 0;
  while (i < code.size()) {
    // Delimiters
    if (code[i] == '{') {
      M.push_back({LBRACE, "{"}), i++;

    } else if (code[i] == '}') {
      M.push_back({RBRACE, "}"}), i++;

    } else if (code[i] == '(') {
      M.push_back({LPAREN, "("}), i++;

    } else if (code[i] == ')') {
      M.push_back({RPAREN, ")"}), i++;

    } else if (code[i] == ',') {
      M.push_back({COMMA, ","}), i++;

    } else if (code[i] == ';') {
      M.push_back({SEMICOLON, ";"}), i++;

      // Operators
    } else if (code[i] == '=') {
      if (code.substr(i, 2) == "==")
        M.push_back({EQUAL, "=="}), i += 2;
      else {
        M.push_back({ASSIGN, "="}), i++;
      }

    } else if (code[i] == '>') {
      M.push_back({GT, ">"}), i++;

    } else if (code[i] == '<') {
      M.push_back({LT, "<"}), i++;

    } else if (code[i] == '+') {
      M.push_back({PLUS, "+"}), i++;

    } else if (code[i] == '-') {
      M.push_back({MINUS, "-"}), i++;

    } else if (code[i] == '/') {
      M.push_back({DIVIDE, "/"}), i++;

    } else if (code[i] == '*') {
      M.push_back({MULTIPLY, "*"}), i++;

    } else if (code[i] == '!') {
      if (code.substr(i, 2) == "!=")
        M.push_back({NOT_EQUAL, "!="}), i += 2;
      else
        M.push_back({BANG, "!"}), i++;

      // Integers
    } else if (isdigit(code[i])) {
      string a = "";
      while (isdigit(code[i]))
        a += code[i], i++;
      if (isalpha(code[i]))
        return {};
      M.push_back({INT, a});

      // Identifiers
    } else if (isalpha(code[i])) {
      string a = "";
      while (isalpha(code[i]))
        a += code[i], i++;

      auto key = keywords.find(a); // Check if keyword
      if (key != keywords.end())
        M.push_back({(*key).second, (*key).first});
      else
        M.push_back({IDENT, a});

    } else if (code[i] == ' ' || code[i] == '\t' ||
               code[i] == '\n') { // Whitespaces
      i++;
    } else {
      M.push_back({ILLEGAL, code.substr(i, 1)});
      return {};
    }
  }
  M.push_back({EOFL, "eof"});
  return M;
}

string program::parseFile() {
  string data;
  ifstream file;
  file.open("sample.mon");

  string line;
  if (file.is_open()) {
    while (getline(file, line)) {
      data += line;
    }
  } else {
    cout << "Couldn't open file\n";
    return "";
  }
  return data;
}

Statement *program ::parseLetStatement() {
  if (peekToken().tokenType != IDENT) {
    cout << "Parsing Error: Expected IDENT. Got "
         << getTokenStr[peekToken().tokenType] << endl;
    return NULL;
  }
  GetNextToken();
  Expression *id = new Identifier(IDENT, currToken().lexeme);

  if (peekToken().tokenType != ASSIGN) {
    cout << "Parsing Error: Expected ASSIGN. Got "
         << getTokenStr[peekToken().tokenType] << endl;
    return NULL;
  }
  GetNextToken();
  GetNextToken();
  Expression *val = parseExpression(LOWEST);

  if (peekToken().tokenType != SEMICOLON) {
    cout << "Parsing Error: Expected SEMICOLON. Got "
         << getTokenStr[peekToken().tokenType] << endl;
    return NULL;
  }
  GetNextToken();

  return (Statement *)(new LetStatement(id, val));
}

Statement *program::parseReturnStatement() {
  // if (peekToken().tokenType != INT) {
  //   cout << "Parsing Error: Expected INT. Got "
  //        << getTokenStr[peekToken().tokenType] << endl;
  //   return NULL;
  // }
  GetNextToken();
  auto stmt = new ReturnStatement(parseExpression(LOWEST));

  if (peekToken().tokenType != SEMICOLON) {
    cout << "Parsing Error: Expected SEMICOLON. Got "
         << getTokenStr[peekToken().tokenType] << endl;
    return NULL;
  }
  GetNextToken();

  return (Statement *)(stmt);
}

Statement *program::parseExpressionStatement() {
  Expression *e = parseExpression(LOWEST);
  if (peekToken().tokenType == SEMICOLON) {
    GetNextToken();
  }
  return (Statement *)(new ExpressionStatement(NULL, e));
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

void program::parseProgram(string data) {
  vector<token> t;
  if (data == "") {
    cout << "I/O Error: Unable to open file!";
    return;
  }

  t = parseTokens(data);
  if (t.size() == 0) {
    cout << "Lexical Error: Invalid Token!";
    return;
  }
  this->tokens = t;

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
  while (peekToken().tokenType != SEMICOLON && precedence < peekPrecedence()) {
    GetNextToken();
    leftExp = parseInfixFn(leftExp);
  }
  return leftExp;
}

Expression *program::parseInfixExpression(Expression *exp) {
  Expression *node =
      new InfixExpression(currToken().tokenType, currToken().lexeme, exp, NULL);
  int p = currPrecedence();
  GetNextToken();
  node->right = parseExpression(p);
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
  GetNextToken();

  node->cond = parseExpression(LOWEST);
  if (peekToken().tokenType != RPAREN) {
    cout << "Parsing Error: Expected RPAREN. Got "
         << getTokenStr[peekToken().tokenType] << endl;
  }

  GetNextToken();
  if (peekToken().tokenType != LBRACE) {
    cout << "Parsing Error: Expected LBRACE. Got "
         << getTokenStr[peekToken().tokenType] << endl;
  }

  GetNextToken();
  node->conseq = parseBlockStatement();

  GetNextToken();
  if (currToken().tokenType == ELSE) {
    GetNextToken();
    if (currToken().tokenType != LBRACE) {
      cout << "Parsing Error: Expected LBRACE. Got "
           << getTokenStr[peekToken().tokenType] << endl;
      return NULL;
    }
    node->altern = parseBlockStatement();
  }
  return node;
}

Expression *program::parseFunctionLiteral() {
  Expression *lit = new FunctionLiteral(currToken().tokenType);
  if (peekToken().tokenType != LPAREN) {
    cout << "Parsing Error: Expected LPAREN. Got "
         << getTokenStr[peekToken().tokenType] << endl;
    return NULL;
  }
  GetNextToken();

  lit->parameters = parseFunctionParameters();
  if (peekToken().tokenType != LBRACE) {
    cout << "Parsing Error: Expected LBRACE. Got "
         << getTokenStr[peekToken().tokenType] << endl;
    return NULL;
  }
  GetNextToken();

  lit->body = parseBlockStatement();
  GetNextToken();
  return lit;
}

vector<Identifier *> program::parseFunctionParameters() {
  vector<Identifier *> id;
  if (peekToken().tokenType == RPAREN) {
    GetNextToken();
    return {};
  }
  GetNextToken();

  Identifier *ident = new Identifier(IDENT, currToken().lexeme);
  id.push_back(ident);

  while (peekToken().tokenType == COMMA) {
    GetNextToken();
    GetNextToken();
    Identifier *ident = new Identifier(IDENT, currToken().lexeme);
    id.push_back(ident);
  }

  if (peekToken().tokenType != RPAREN) {
    cout << "Parsing Error: Expected RPAREN. Got "
         << getTokenStr[peekToken().tokenType] << endl;
    return {};
  }
  GetNextToken();
  return id;
}

Expression *program::parseCallExpression(Expression *function) {
  Expression *exp = new CallExpression(currToken().tokenType, function);
  exp->arguments = parseCallArguements();
  return exp;
}

vector<Expression *> program::parseCallArguements() {
  vector<Expression *> args;
  if (peekToken().tokenType == RPAREN) {
    GetNextToken();
    return {};
  }

  GetNextToken();
  args.push_back(parseExpression(LOWEST));
  while (peekToken().tokenType == COMMA) {
    GetNextToken();
    GetNextToken();
    args.push_back(parseExpression(LOWEST));
  }

  if (peekToken().tokenType != RPAREN) {
    cout << "Parsing Error: Expected RPAREN. Got "
         << getTokenStr[peekToken().tokenType] << endl;
    return {};
  }
  GetNextToken();

  return args;
}

Expression *program::parseInfixFn(Expression *exp) {
  switch (currToken().tokenType) {
  case LPAREN:
    return parseCallExpression(exp);

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

  case FUNCTION:
    return parseFunctionLiteral();

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

// int main(int argc, char *argv[]) {
//   program p;
//   p.parseProgram(p.parseFile());
//   return 0;
// }
