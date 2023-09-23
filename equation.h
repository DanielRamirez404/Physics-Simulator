#pragma once
#include "operation.h"
#include "formula.h"
#include "usermath.h"
#include "userstring.h"
#include "math characters.h"
#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace Math {
  using namespace Operators;

  enum class EquationSide {
    right,
    left,
  };

  template <typename T> struct Variable {
    char identifier{};
    T value{};
  };

  template <typename T> class Equation {
  private:
    std::string formula{};
    std::vector<Variable<T>> variables{};
    size_t variableCounter{};
    std::string_view getSideView(EquationSide side) { return (side == EquationSide::right) ? getRightSideView() : getLeftSideView(); };
    std::string_view getLeftSideView();
    std::string_view getRightSideView();
    void rewriteFormulaToSolveFor(char identifier);
  public:
    Equation(std::string_view myFormula, std::vector<char> myVariableNames) : formula(myFormula) 
    {
      String::eraseWhitespaces(formula);
      for (size_t i{0}; i < myVariableNames.size(); ++i) {
        //assert(!isMathRelated(i) && i != '=' && "INVALID VARIABLE IDENTIFIER");
        variables.push_back ( { myVariableNames[i] } );
      }
      variableCounter = variables.size();
      //assert((variableCounter > 0) && "FORMULA MUST HAVE AT LEAST ONE IDENTIFIER");
    };
    T solveFor(char identifier);
    void addValueFor(char identifier, T value);
  };
  Equation(std::string_view, std::vector<char>) -> Equation<double>;

  template <typename T> std::string_view Equation<T>::getLeftSideView() {
    std::string_view leftOperand{formula};
    leftOperand.remove_suffix(formula.size() - String::findIndexOfCharacter(formula, '='));
    return leftOperand;
  }

  template <typename T> std::string_view Equation<T>::getRightSideView() {
    std::string_view leftOperand{formula};
    leftOperand.remove_prefix(String::findIndexOfCharacter(formula, '=') + 1);
    return leftOperand;
  }

  template <typename T> T Equation<T>::solveFor(char identifier) {
    //assert( (formula.find(identifier) != std::string::npos) && "IDENTIFIER DOES NOT EXIST");
    //assert( (variableCounter == 1) && "THERE CANNOT BE MORE THAN ONE UNKNOWN VARIABLE IN THE FORMULA");
    std::string_view leftSide{ getLeftSideView() };
    std::string_view rightSide{ getRightSideView() };
    if (leftSide[0] == identifier && leftSide.size() == 1) {
      Operation<T> result { rightSide };
      return result.solve();
    }
    if (rightSide[0] == identifier && rightSide.size() == 1) {
      Operation<T> result { leftSide };
      return result.solve();
    }
    rewriteFormulaToSolveFor(identifier);
    return solveFor(identifier);
  }

  template <typename T> void Equation<T>::addValueFor(char identifier, T value) {
    size_t identifierIndex { formula.find(identifier) };
    //assert( (identifierIndex != std::string::npos) && "IDENTIFIER DOES NOT EXIST");
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
          std::string oppositeOperator(1, getOpposite(formula[i + 1]) );
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
