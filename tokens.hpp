#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

// Token types
enum tokentype {
  LET,
  FUNCTION,
  RETURN,
  IF,
  ELSE,
  TRUE,
  FALSE,
  LBRACE,
  RBRACE,
  LPAREN,
  RPAREN,
  COMMA,
  SEMICOLON,
  EQUAL,
  ASSIGN,
  GT,
  LT,
  PLUS,
  MINUS,
  DIVIDE,
  MULTIPLY,
  NOT_EQUAL,
  BANG,
  INT,
  IDENT,
  ILLEGAL,
  EOFL
};

class token {
public:
  tokentype tokenType;
  string lexeme;
};

token GetNextToken(vector<token> &tokens) {
  token temp = tokens[0];
  tokens.erase(tokens.begin());
  return temp;
}

token currToken(vector<token> &t) { return t[0]; }

token peekToken(vector<token> &t) { return t[1]; }

unordered_map<string, tokentype> keywords = {
    {"let", LET},   {"fn", FUNCTION}, {"if", IF},        {"else", ELSE},
    {"true", TRUE}, {"false", FALSE}, {"return", RETURN}};

unordered_map<tokentype, string> getTokenStr = {{LET, "LET"},
                                                {FUNCTION, "FUNCTION"},
                                                {RETURN, "RETURN"},
                                                {IF, "IF"},
                                                {ELSE, "ELSE"},
                                                {TRUE, "TRUE"},
                                                {FALSE, "FALSE"},
                                                {LBRACE, "LBRACE"},
                                                {RBRACE, "RBRACE"},
                                                {LPAREN, "LPAREN"},
                                                {RPAREN, "RPAREN"},
                                                {COMMA, "COMMA"},
                                                {SEMICOLON, "SEMICOLON"},
                                                {EQUAL, "EQUAL"},
                                                {ASSIGN, "ASSIGN"},
                                                {GT, "GT"},
                                                {LT, "LT"},
                                                {PLUS, "PLUS"},
                                                {MINUS, "MINUS"},
                                                {DIVIDE, "DIVIDE"},
                                                {MULTIPLY, "MULTIPLY"},
                                                {NOT_EQUAL, "NOT_EQUAL"},
                                                {BANG, "BANG"},
                                                {INT, "INT"},
                                                {IDENT, "IDENT"},
                                                {ILLEGAL, "ILLEGAL"},
                                                {EOFL, "EOF"}};
