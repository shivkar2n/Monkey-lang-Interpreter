#include "ast.hpp"
#include <iostream>

class Parser {
public:
  std::vector<Statement *> statements;
  std::vector<Token> tokens;

  Token get_next_token() {
    Token temp = tokens[0];
    tokens.erase(tokens.begin());
    return temp;
  }
  Token curr_token() { return tokens[0]; }
  Token peek_token() { return tokens[1]; }

  int curr_precedence() { return tokenPrecedence[curr_token().tokenType]; }
  int peek_predence() { return tokenPrecedence[peek_token().tokenType]; }
  std::string curr_tokenLiteral() {
    return getTokenStr[curr_token().tokenType];
  }

  Statement *parse_statement();
  Statement *parse_return_statement();
  Statement *parse_let_statement();
  Statement *parse_expr_statement();
  BlockStatement *parse_block_statement();

  Expression *parse_expression(int precedence);
  Expression *parse_infix_expression(Expression *exp);
  Expression *parse_prefix_expression();
  Expression *parse_group_expression();
  Expression *parse_if_expression();
  Expression *parse_function_literal();
  std::vector<Identifier *> parse_function_parameters();

  Expression *parse_call_expression(Expression *exp);
  std::vector<Expression *> parseCallArguements();

  Expression *parse_infix_expr(Expression *exp);
  Expression *parse_prefix_expr(Tokentype tokenType);

  Expression *parse_identifier();
  Expression *parse_integer_literal();
  Expression *parse_boolean();

  void parse_program(std::vector<Token> tokens);
};

Statement *Parser ::parse_let_statement() {
  if (peek_token().tokenType != IDENT) {
    std::cout << "Parsing Error: Expected IDENT. Got "
              << getTokenStr[peek_token().tokenType] << "\n";
    return NULL;
  }
  get_next_token();
  Expression *id = new Identifier(IDENT, curr_token().lexeme);

  if (peek_token().tokenType != ASSIGN) {
    std::cout << "Parsing Error: Expected ASSIGN. Got "
              << getTokenStr[peek_token().tokenType] << "\n";
    return NULL;
  }
  get_next_token();
  get_next_token();
  Expression *val = parse_expression(LOWEST);

  if (peek_token().tokenType != SEMICOLON) {
    std::cout << "Parsing Error: Expected SEMICOLON. Got "
              << getTokenStr[peek_token().tokenType] << "\n";
    return NULL;
  }
  get_next_token();

  return (Statement *)(new LetStatement(id, val));
}

Statement *Parser::parse_return_statement() {
  // if (peek_token().tokenType != INT) {
  //  std::cout << "Parsing Error: Expected INT. Got "
  //        << getTokenStr[peek_token().tokenType] << "\n";
  //   return NULL;
  // }
  get_next_token();
  auto stmt = new ReturnStatement(parse_expression(LOWEST));

  if (peek_token().tokenType != SEMICOLON) {
    std::cout << "Parsing Error: Expected SEMICOLON. Got "
              << getTokenStr[peek_token().tokenType] << "\n";
    return NULL;
  }
  get_next_token();

  return (Statement *)(stmt);
}

Statement *Parser::parse_expr_statement() {
  Expression *e = parse_expression(LOWEST);
  if (peek_token().tokenType == SEMICOLON) {
    get_next_token();
  }
  return (Statement *)(new ExpressionStatement(NULL, e));
}

Statement *Parser ::parse_statement() {
  if (curr_token().tokenType == LET) {
    return parse_let_statement();
  } else if (curr_token().tokenType == RETURN) {
    return parse_return_statement();
  } else {
    return parse_expr_statement();
  }
}

void Parser::parse_program(std::vector<Token> tokens) {
  this->tokens = tokens;
  while (curr_token().tokenType != EOFL) {
    statements.push_back(parse_statement());
    get_next_token();
  }
}

BlockStatement *Parser::parse_block_statement() {
  BlockStatement *block = new BlockStatement(curr_token().tokenType);
  get_next_token();
  while (curr_token().tokenType != RBRACE && curr_token().tokenType != EOFL) {
    block->statements.push_back(parse_statement());
    get_next_token();
  }
  return block;
}

Expression *Parser::parse_expression(int precedence) {
  Expression *leftExp = parse_prefix_expr(curr_token().tokenType);
  if (leftExp == NULL) {
    std::cout << "Parsing Error: No prefix function called!!"
              << "\n";
    return NULL;
  }
  while (peek_token().tokenType != SEMICOLON && precedence < peek_predence()) {
    get_next_token();
    leftExp = parse_infix_expr(leftExp);
  }
  return leftExp;
}

Expression *Parser::parse_infix_expression(Expression *exp) {
  Expression *node = new InfixExpression(curr_token().tokenType,
                                         curr_token().lexeme, exp, NULL);
  int p = curr_precedence();
  get_next_token();
  node->right = parse_expression(p);
  return node;
}

Expression *Parser::parse_prefix_expression() {
  Expression *node =
      new PrefixExpression(curr_token().tokenType, curr_token().lexeme, NULL);
  get_next_token();
  node->right = parse_expression(PREFIX);
  return node;
}

Expression *Parser::parse_group_expression() {
  get_next_token();
  Expression *exp = parse_expression(LOWEST);
  if (peek_token().tokenType != RPAREN) {
    std::cout << "Parsing Error: Expected RPAREN. Got "
              << getTokenStr[peek_token().tokenType] << "\n";
  }
  get_next_token();
  return exp;
}

Expression *Parser::parse_if_expression() {
  Expression *node = new IfExpression(curr_token().tokenType, NULL, NULL, NULL);
  if (peek_token().tokenType != LPAREN) {
    std::cout << "Parsing Error: Expected LPAREN. Got "
              << getTokenStr[peek_token().tokenType] << "\n";
  }
  get_next_token();
  get_next_token();

  node->cond = parse_expression(LOWEST);
  if (peek_token().tokenType != RPAREN) {
    std::cout << "Parsing Error: Expected RPAREN. Got "
              << getTokenStr[peek_token().tokenType] << "\n";
  }

  get_next_token();
  if (peek_token().tokenType != LBRACE) {
    std::cout << "Parsing Error: Expected LBRACE. Got "
              << getTokenStr[peek_token().tokenType] << "\n";
  }

  get_next_token();
  node->conseq = parse_block_statement();

  get_next_token();
  if (curr_token().tokenType == ELSE) {
    get_next_token();
    if (curr_token().tokenType != LBRACE) {
      std::cout << "Parsing Error: Expected LBRACE. Got "
                << getTokenStr[peek_token().tokenType] << "\n";
      return NULL;
    }
    node->altern = parse_block_statement();
  }
  return node;
}

Expression *Parser::parse_function_literal() {
  Expression *lit = new FunctionLiteral(curr_token().tokenType);
  if (peek_token().tokenType != LPAREN) {
    std::cout << "Parsing Error: Expected LPAREN. Got "
              << getTokenStr[peek_token().tokenType] << "\n";
    return NULL;
  }
  get_next_token();

  lit->parameters = parse_function_parameters();
  if (peek_token().tokenType != LBRACE) {
    std::cout << "Parsing Error: Expected LBRACE. Got "
              << getTokenStr[peek_token().tokenType] << "\n";
    return NULL;
  }
  get_next_token();

  lit->body = parse_block_statement();
  get_next_token();
  return lit;
}

std::vector<Identifier *> Parser::parse_function_parameters() {
  std::vector<Identifier *> id;
  if (peek_token().tokenType == RPAREN) {
    get_next_token();
    return {};
  }
  get_next_token();

  Identifier *ident = new Identifier(IDENT, curr_token().lexeme);
  id.push_back(ident);

  while (peek_token().tokenType == COMMA) {
    get_next_token();
    get_next_token();
    Identifier *ident = new Identifier(IDENT, curr_token().lexeme);
    id.push_back(ident);
  }

  if (peek_token().tokenType != RPAREN) {
    std::cout << "Parsing Error: Expected RPAREN. Got "
              << getTokenStr[peek_token().tokenType] << "\n";
    return {};
  }
  get_next_token();
  return id;
}

Expression *Parser::parse_call_expression(Expression *function) {
  Expression *exp = new CallExpression(curr_token().tokenType, function);
  exp->arguments = parseCallArguements();
  return exp;
}

std::vector<Expression *> Parser::parseCallArguements() {
  std::vector<Expression *> args;
  if (peek_token().tokenType == RPAREN) {
    get_next_token();
    return {};
  }

  get_next_token();
  args.push_back(parse_expression(LOWEST));
  while (peek_token().tokenType == COMMA) {
    get_next_token();
    get_next_token();
    args.push_back(parse_expression(LOWEST));
  }

  if (peek_token().tokenType != RPAREN) {
    std::cout << "Parsing Error: Expected RPAREN. Got "
              << getTokenStr[peek_token().tokenType] << "\n";
    return {};
  }
  get_next_token();

  return args;
}

Expression *Parser::parse_infix_expr(Expression *exp) {
  switch (curr_token().tokenType) {
  case LPAREN:
    return parse_call_expression(exp);

  case GT:
    return parse_infix_expression(exp);

  case LT:
    return parse_infix_expression(exp);

  case NOT_EQUAL:
    return parse_infix_expression(exp);

  case EQUAL:
    return parse_infix_expression(exp);

  case PLUS:
    return parse_infix_expression(exp);

  case DIVIDE:
    return parse_infix_expression(exp);

  case MULTIPLY:
    return parse_infix_expression(exp);

  default:
    return NULL;
  }
}

Expression *Parser::parse_prefix_expr(Tokentype type) {
  switch (type) {

  case FUNCTION:
    return parse_function_literal();

  case IF:
    return parse_if_expression();

  case IDENT:
    return parse_identifier();

  case INT:
    return parse_integer_literal();

  case MINUS:
    return parse_prefix_expression();

  case BANG:
    return parse_prefix_expression();

  case TRUE:
    return parse_boolean();

  case FALSE:
    return parse_boolean();

  case LPAREN:
    return parse_group_expression();

  default:
    return NULL;
  }
}

Expression *Parser::parse_identifier() {
  return new Identifier(curr_token().tokenType, curr_token().lexeme);
}

Expression *Parser::parse_integer_literal() {
  return new Literal(curr_token().tokenType, stoi(curr_token().lexeme));
}

Expression *Parser::parse_boolean() {
  return new Boolean(curr_token().tokenType, curr_token().lexeme == "true");
}