enum ObjectType { INTEGER, BOOLEAN, NULLVAL, RETURNVAL };

class Object {
public:
  ObjectType type;
};

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
  int value;

public:
  ReturnLiteral(int value) {
    this->type = RETURNVAL;
    this->value = value;
  }
};
