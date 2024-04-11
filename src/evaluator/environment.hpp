#include "object.hpp"
#include <string>
#include <unordered_map>

#if !defined(EVAL_ENVIRONMENT_HPP)
#define EVAL_ENVIRONMENT_HPP
class Object;

class Environment {
  std::unordered_map<std::string, Object *> store;
  Environment *outer;

public:
  Environment();
  Environment(Environment *outer);
  Object *get(std::string name);
  Object *set(std::string name, Object *val);
};

Environment::Environment() { this->outer = NULL; }
Environment::Environment(Environment *outer) { this->outer = outer; }

Object *Environment::get(std::string name) {
  auto obj = store.count(name);
  if (!obj && outer != NULL) {
    return outer->get(name);
  }
  return store[name];
}

Object *Environment::set(std::string name, Object *val) {
  store[name] = val;
  return store[name];
}
#endif