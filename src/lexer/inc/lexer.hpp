#include "tokens.hpp"
#include <vector>

#if !defined(LEXER_LEXER_HPP)
#define LEXER_LEXER_HPP
class Lexer {
public:
  std::vector<Token> parse_tokens(std::string line);
};

#endif // LEXER_LEXER_HPP
