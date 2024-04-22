#include "lexer.hpp"

#if !defined(LEXER_LEXER_CPP)
#define LEXER_LEXER_CPP

std::vector<Token> Lexer::parse_tokens(std::string line) {
  std::vector<Token> tokenStream;
  int i = 0;
  while (i < line.size()) {
    // Delimiters
    if (line[i] == '{') {
      tokenStream.push_back({LBRACE, "{"}), i++;

    } else if (line[i] == '}') {
      tokenStream.push_back({RBRACE, "}"}), i++;

    } else if (line[i] == '(') {
      tokenStream.push_back({LPAREN, "("}), i++;

    } else if (line[i] == ')') {
      tokenStream.push_back({RPAREN, ")"}), i++;

    } else if (line[i] == ',') {
      tokenStream.push_back({COMMA, ","}), i++;

    } else if (line[i] == ';') {
      tokenStream.push_back({SEMICOLON, ";"}), i++;

      // Operators
    } else if (line[i] == '=') {
      if (line.substr(i, 2) == "==")
        tokenStream.push_back({EQUAL, "=="}), i += 2;
      else {
        tokenStream.push_back({ASSIGN, "="}), i++;
      }

    } else if (line[i] == '>') {
      tokenStream.push_back({GT, ">"}), i++;

    } else if (line[i] == '<') {
      tokenStream.push_back({LT, "<"}), i++;

    } else if (line[i] == '+') {
      tokenStream.push_back({PLUS, "+"}), i++;

    } else if (line[i] == '-') {
      tokenStream.push_back({MINUS, "-"}), i++;

    } else if (line[i] == '/') {
      tokenStream.push_back({DIVIDE, "/"}), i++;

    } else if (line[i] == '*') {
      tokenStream.push_back({MULTIPLY, "*"}), i++;

    } else if (line[i] == '!') {
      if (line.substr(i, 2) == "!=")
        tokenStream.push_back({NOT_EQUAL, "!="}), i += 2;
      else
        tokenStream.push_back({BANG, "!"}), i++;

      // Integers
    } else if (isdigit(line[i])) {
      std::string a = "";
      while (isdigit(line[i]))
        a += line[i], i++;
      if (isalpha(line[i]))
        return {};
      tokenStream.push_back({INT, a});

      // Identifiers
    } else if (isalpha(line[i])) {
      std::string a = "";
      while (isalpha(line[i]))
        a += line[i], i++;

      auto key = keywords.find(a); // Check if keyword
      if (key != keywords.end())
        tokenStream.push_back({(*key).second, (*key).first});
      else
        tokenStream.push_back({IDENT, a});

    } else if (line[i] == ' ' || line[i] == '\t' ||
               line[i] == '\n') { // Whitespaces
      i++;
    } else {
      tokenStream.push_back({ILLEGAL, line.substr(i, 1)});
      return {};
    }
  }
  tokenStream.push_back({EOFL, "eof"});
  return tokenStream;
}

#endif // LEXER_LEXER_CPP