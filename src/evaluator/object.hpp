enum ObjectType { INTEGER, BOOLEAN, NULLVAL };

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
