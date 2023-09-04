#pragma once
#include "operation.h"
#include "usermath.h"
#include <string>
#include <string_view>

namespace Math {
  template <typename T> struct Variable {
    char identifier{};
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
    T solveFor(char identifier);
  };

  template <typename T> T Ecuation<T>::solveFor(char identifier) {
    /*  if (doesIndentifierExist(identifier))
     *  1. operation = Ecuation.getOperationToCalculate(identifier)
     *  2. return operation.solve();
     */
  }
}
