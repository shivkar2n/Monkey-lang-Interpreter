#include "interpreter/interpreter.hpp"

int main(int argc, char const *argv[]) {
  std::string PROMPT = ">>";
  std::string line = " ";

  while (line != "") {
    try {
      std::cout << PROMPT;
      std::getline(std::cin, line);
      Interpreter interpreter;
      interpreter.run(line);
      std::cout << line << '\n';
    } catch (const std::exception &e) {
      std::cerr << e.what() << '\n';
    }
  }
  return 0;
}
