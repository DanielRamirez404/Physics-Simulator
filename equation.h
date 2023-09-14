#pragma once
#include "operation.h"
#include "formula.h"
#include "usermath.h"
#include "userstring.h"
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
    void rewriteFormulaToSolveFor(char identifier);
  public:
    Equation(const char* myFormula, std::vector<char> myVariableNames) : formula(myFormula) 
    {
      for (size_t i{0}; i < myVariableNames.size(); ++i) {
        assert(!isMathRelated(i) && i != '=' && "INVALID VARIABLE IDENTIFIER");
        variables.push_back ( { myVariableNames[i]} );
      }
      variableCounter = variables.size();
      assert((variableCounter > 0) && "FORMULA MUST HAVE AT LEAST ONE IDENTIFIER");
      Formula::removeWhitespaces(formula);
    };
    Equation(const Equation&) = delete;
    Equation& operator=(const Equation&) = delete;
    T solveFor(char identifier);
    void addValueFor(char identifier, T value);
  };

  template <typename T> T Equation<T>::solveFor(char identifier) {
    assert( (formula.find(identifier) != std::string::npos) && "IDENTIFIER DOES NOT EXIST");
    assert( (variableCounter == 1) && "THERE CANNOT BE MORE THAN ONE UNKNOWN VARIABLE IN THE FORMULA");
    if (formula[0] == identifier && formula[1] == '=') {
      constexpr int firstRightExpressionIndex{2};
      Operation<T> result { formula.substr(firstRightExpressionIndex) };
      return result.solve();
    }
    rewriteFormulaToSolveFor(identifier);
    return solveFor(identifier);
  }

  template <typename T> void Equation<T>::addValueFor(char identifier, T value) {
    size_t identifierIndex { formula.find(identifier) };
    assert( (identifierIndex != std::string::npos) && "IDENTIFIER DOES NOT EXIST");
    constexpr size_t charSize { 1 };
    formula.replace(identifierIndex, charSize, std::to_string(value));
    --variableCounter;
  }

  template <typename T> void Equation<T>::rewriteFormulaToSolveFor(char identifier) {
    //needs HUGE rework
    bool isIdentifierToTheLeft { false } ;
    //getValueForPreviousBool
    for (size_t i{0}; formula[i] != '='; ++i) {
      if (formula[i] == identifier) {
        isIdentifierToTheLeft = true;
        break;
      }
    }
    //assuming there's no parenthesis and that the operator is to the right
    if (isIdentifierToTheLeft) {
      //place parentheses at the right side of the equal sign
      for (size_t i{0}; true; ++i) {
        if (formula[i] == '=') {
          String::addToString(formula, "(", i + 1);
          formula.append(")"); 
          break;
        }
      }
      //change the operator and pass it to the right with the number
      for (size_t i{0}; true; ++i) {
        if (formula[i] == identifier) {
          std::string oppositeOperator(1, getOppositeOperator(formula[i + 1]) );
          formula.append(oppositeOperator);
          formula.erase(i + 1, 1);
          //pasted from getNumber() function in the operation class
          size_t iterator{ i + 1 };
          const size_t firstDigit { iterator };
          while (isNumeric(formula[iterator + 1])) ++iterator;
          const size_t firstNonDigit { iterator + 1};
          const size_t totalDigits { firstNonDigit - firstDigit };
          std::string numberString{ formula.substr(firstDigit, totalDigits) };
          //we got our number so we append and erase it
          formula.append(numberString);
          formula.erase(firstDigit, totalDigits);
          break;
        }
      }
    }
  }
}
