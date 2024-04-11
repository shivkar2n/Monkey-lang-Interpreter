#if !defined(EVALUATOR_OBJECT_HPP)
#define EVALUATOR_OBJECT_HPP
class Environment;

enum ObjectType { INTEGER, BOOLEAN, NULLVAL, RETURNVAL, ERROR, FUNCTIONOBJ };

std::unordered_map<ObjectType, std::string> get_object_string = {
    {INTEGER, "INTEGER"},     {BOOLEAN, "BOOLEAN"}, {NULLVAL, "NULLVAL"},
    {RETURNVAL, "RETURNVAL"}, {ERROR, "ERROR"},     {FUNCTIONOBJ, "FUNCTION"},
};

class Object {
public:
  ObjectType type;

  std::string get_type() { return get_object_string[type]; }
};

bool is_error(Object *object) {
  if (object != NULL) {
    return object->type == ERROR;
  }
  return false;
}

class IntegerLiteral : Object {
public:
  int value;
  IntegerLiteral(int value) {
    this->type = INTEGER;
    this->value = value;
  }
};

class BooleanLiteral : Object {
public:
  bool value;
  BooleanLiteral(int value) {
    this->type = BOOLEAN;
    this->value = value != 0;
  }
};

class NullLiteral : Object {
public:
  NullLiteral() { this->type = NULLVAL; }
};

class ReturnLiteral : Object {
public:
  Object *value;
  ReturnLiteral(Object *value) {
    this->type = RETURNVAL;
    this->value = value;
  }
};

class Error : Object {
public:
  std::string value;
  Error(std::string value) {
    this->type = ERROR;
    this->value = value;
  }
};

class Function : Object {
public:
  std::vector<Identifier *> parameters;
  BlockStatement *body;
  Environment *env;

  Function(std::vector<Identifier *> parameters, BlockStatement *body,
           Environment *env

  ) {
    this->type = FUNCTIONOBJ;
    this->parameters = parameters;
    this->body = body;
    this->env = env;
  }
};

#endif