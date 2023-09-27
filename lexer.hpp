#include "tokens.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
using namespace std;

int parseTokens(string code, vector<token> &M) {
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
        return -1;
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
      return -1;
    }
  }
  M.push_back({EOFL, "eof"});
  return 0;
}

int parseFile(string &data) {
  ifstream file;
  file.open("sample.mon");

  string line;
  if (file.is_open()) {
    while (file) {
      getline(file, line);
      data += line;
    }
  } else {
    cout << "Couldn't open file\n";
    return -1;
  }
  return 0;
}
