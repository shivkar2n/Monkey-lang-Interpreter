#include "ast.hpp"
#include "lexer.hpp"
using namespace std;

void parseExpression() {}

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
  // string value = parseExpression();

  if (peekToken(t).tokenType != SEMICOLON) {
    cout << "Parsing Error: Expected SEMICOLON. Got "
         << getTokenStr[peekToken(t).tokenType];
    return -1;
  }
  GetNextToken(t);

  Statement s;
  s.letStatement = new LetStatement(id, val);
  p.statements.push_back(s);
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

  if (peekToken(t).tokenType != SEMICOLON) {
    cout << "Parsing Error: Expected SEMICOLON. Got "
         << getTokenStr[peekToken(t).tokenType];
    return -1;
  }
  GetNextToken(t);

  Statement s;
  s.returnStatement = new ReturnStatement(val);
  p.statements.push_back(s);
  return 0;
}

int parseStatement(program &p, vector<token> &t) {
  while (currToken(t).tokenType != EOFL) {
    if (currToken(t).tokenType == LET) {
      if (parseLetStatement(p, t) == -1)
        return -1;
    } else if (currToken(t).tokenType == RETURN) {
      if (parseReturnStatement(p, t) == -1)
        return -1;
    } else
      parseExpression();
    GetNextToken(t);
  }
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
