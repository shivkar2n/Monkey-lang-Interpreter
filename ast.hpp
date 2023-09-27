using namespace std;

// Precendences
enum precedence { LOWEST, EQUALS, LESSGREATER, SUM, PRODUCT, PREFIX, CALL };

// Map token to given predence
unordered_map<tokentype, precedence> tokenPrecedence = {
    {EQUAL, EQUALS},   {NOT_EQUAL, EQUALS}, {GT, LESSGREATER},
    {LT, LESSGREATER}, {PLUS, SUM},         {MINUS, SUM},
    {DIVIDE, PRODUCT}, {MULTIPLY, PRODUCT}

};

class LetStatement {
public:
  string identifier;
  string value;
  LetStatement(string id, string val) {
    identifier = id;
    value = val;
  }
};

class ReturnStatement {
public:
  string value;
  ReturnStatement(string val) { value = val; }
};

class ExpressionStatement {
public:
  string value;
  ExpressionStatement(string val) { value = val; }
};

class Statement {
public:
  LetStatement *letStatement;
  ReturnStatement *returnStatement;
  ExpressionStatement *expressionStatement;
};

// Main root node
class program {
public:
  vector<Statement> statements;
};