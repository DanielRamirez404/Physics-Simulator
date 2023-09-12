#pragma once
#include "operation.h"
#include "usermath.h"
#include <cassert>
#include <cstddef>
#include <string>
#include <vector>

namespace Math {
  template <typename T> struct Variable {
    char identifier{};
    T value{};
  };

  template <typename T> class Equation {
  private:
    std::string formula{};
    std::vector<Variable<T>> variables{};
    size_t variableCounter{};
  public:
    Equation(const char* myFormula, std::vector<char> myVariableNames) : formula(myFormula) 
    {
      for (size_t i{0}; i < myVariableNames.size(); ++i) {
        variables.push_back ( { myVariableNames[i], 0 } );
      }
      variableCounter = variables.size();
      assert((variableCounter > 0) && "FORMULA MUST HAVE AT LEAST ONE IDENTIFIER");
    };
    Equation(const Equation&) = delete;
    Equation& operator=(const Equation&) = delete;
    T solveFor(char identifier);
  };

  template <typename T> T Equation<T>::solveFor(char identifier) {
    /*  todo:
     *  1. if (doesIndentifierExist(identifier)) { 
     *  2.   operation myOperation { Equation.getOperationToCalculate(identifier) };
     *  3.   return operation.getResult();
     *     }
     */
  }
}
