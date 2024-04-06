#include "object.hpp"
#include <string>
#include <unordered_map>

class Environment {
  std::unordered_map<std::string, Object *> store;

public:
  Object *get(std::string name);
  Object *set(std::string name, Object *val);
};

Object *Environment::get(std::string name) {
  if (!store.count(name)) {
    return NULL;
  }
  return store[name];
}

Object *Environment::set(std::string name, Object *val) {
  store[name] = val;
  return store[name];
}
