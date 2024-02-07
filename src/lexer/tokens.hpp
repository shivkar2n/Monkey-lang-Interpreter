#if !defined(LEXER_TOKEN_HPP)
#define LEXER_TOKEN_HPP

#include <string>
#include <unordered_map>

enum Tokentype {
  // Keywords
  LET,
  FUNCTION,
  RETURN,
  IF,
  ELSE,
  TRUE,
  FALSE,

  // Delimiters
  LBRACE,
  RBRACE,
  LPAREN,
  RPAREN,
  COMMA,
  SEMICOLON,

  // Operators
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

  // Special
  IDENT,
  ILLEGAL,
  EOFL
};

class Token {
public:
  Tokentype tokenType;
  std::string lexeme;
};

std::unordered_map<std::string, Tokentype> keywords = {
    {"let", LET},   {"fn", FUNCTION}, {"if", IF},        {"else", ELSE},
    {"true", TRUE}, {"false", FALSE}, {"return", RETURN}};

std::unordered_map<Tokentype, std::string> getTokenStr = {
    {LET, "LET"},
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

#endif // LEXER_TOKEN_HPP