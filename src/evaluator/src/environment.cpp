#include "environment.hpp"

#if !defined(EVAL_ENVIRONMENT_CPP)
#define EVAL_ENVIRONMENT_CPP
Environment::Environment() { this->outer = NULL; }
Environment::Environment(Environment *outer) { this->outer = outer; }

Object *Environment::get(std::string name)
{
  auto obj = store.count(name);
  if (!obj && outer != NULL)
  {
    return outer->get(name);
  }
  return store[name];
}

Object *Environment::set(std::string name, Object *val)
{
  store[name] = val;
  return store[name];
}
#endif