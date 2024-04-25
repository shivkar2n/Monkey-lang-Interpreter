#if !defined(EVALUATOR_OBJECT_HPP)
#define EVALUATOR_OBJECT_HPP
class Environment;
class Object;

enum ObjectType
{
  UNINTITIALIZED,
  INTEGER,
  BOOLEAN,
  NULLVAL,
  RETURNVAL,
  ERROR,
  FUNCTIONOBJ
};

std::unordered_map<ObjectType, std::string> get_object_string = {
    {INTEGER, "INTEGER"},
    {BOOLEAN, "BOOLEAN"},
    {NULLVAL, "NULLVAL"},
    {RETURNVAL, "RETURNVAL"},
    {ERROR, "ERROR"},
    {FUNCTIONOBJ, "FUNCTION"},
};

struct IntegerLiteral
{
  int value;
};

struct BooleanLiteral
{
  bool value;
};

struct NullLiteral
{
};

struct ReturnLiteral
{
public:
  Object *value;
};

struct Error
{
  std::string value;
};

struct Function
{
  std::vector<Identifier *> parameters;
  BlockStatement *body;
  Environment *env;
};

class Object
{
public:
  ObjectType type = UNINTITIALIZED;
  union
  {
    IntegerLiteral *integerLiteral;
    BooleanLiteral *booleanLiteral;
    NullLiteral *nullLiteral;
    ReturnLiteral *returnLiteral;
    Error *error;
    Function *function;
  };
  std::string get_type() { return get_object_string[type]; }
};

bool is_error(Object *object)
{
  if (object->type != UNINTITIALIZED)
    return object->type == ERROR;
  return false;
}

#endif