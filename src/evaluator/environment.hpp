#include "object.hpp"
#include <string>
#include <unordered_map>

#if !defined(EVAL_ENVIRONMENT_HPP)
#define EVAL_ENVIRONMENT_HPP

class Environment
{
  std::unordered_map<std::string, Object *> store;
  Environment *outer;

public:
  Environment();
  Environment(Environment *outer);
  Object *get(std::string name);
  Object *set(std::string name, Object *val);
};
#endif