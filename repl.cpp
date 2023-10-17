#include "parser.cpp"
using namespace std;

int main(int argc, char const *argv[]) {
  string PROMPT = ">>";
  string line = " ";

  while (line != "") {
    cout << PROMPT;
    getline(cin, line);
    program p;
    p.parseProgram(line);
    cout << line << endl;
  }
  return 0;
}
