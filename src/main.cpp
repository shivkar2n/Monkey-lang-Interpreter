#include "interpreter/interpreter.hpp"

int main(int argc, char const *argv[]) {
  Interpreter interpreter;
  std::string PROMPT = ">>";
  std::string line = " ";

  while (line != "") {
    try {
      std::cout << PROMPT;
      std::getline(std::cin, line);
      interpreter.run(line);

    } catch (std ::string msg) {
      std::cerr << msg << '\n';
    }
  }
  return 0;
}
