#include "lexer.hpp"
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char const *argv[]) {
  string PROMPT = ">>";
  string line = " ";

  while (line != "") {
    cout << PROMPT;
    getline(cin, line);
    vector<token> P;
    if (parseTokens(line, P) == -1) {
      cout << "Error: Unable to parse tokens";
    } else {
      for (auto &i : P) {
        cout << i.tokenType << " " << i.lexeme << endl;
      }
    }
  }
  return 0;
}
