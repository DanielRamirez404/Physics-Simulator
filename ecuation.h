#pragma once
#include "math.h"
#include <string>
#include <string_view>

template <typename T> struct Variable {
  std::string_view name{};
  T value{};
};

template <typename T> class Ecuation {
private:
  std::string formula{};
  Variable* variables{};
public:
  Ecuation(const char* myFormula, std::string_view* myVariableNames) : formula(myFormula), variableNames(myVariableNames) {};
  Ecuation(const Ecuation&) = delete;
  Ecuation& operator=(const Ecuation&) = delete;
  T solveFor();
};
