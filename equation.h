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
#include <algorithm>

namespace Math {
  using namespace Operators;

  enum class Side {
    left,
    right,
  };

  Side getOppositeSide(Side side) {
    return (side == Side::left) ? Side::right : Side::left;
  }

  template <typename T> struct Variable {
    char identifier{};
    T value{};
  };

  template <typename T> class Equation {
  private:
    std::string formula{};
    std::vector<Variable<T>> variables{};
    size_t variableCounter{};
    size_t getEqualsSignIndex() { return formula.find('='); };
    size_t getFirstIndexOfSide(Side side) { return (side == Side::left) ? 0 : getEqualsSignIndex() + 1; };
    size_t getLastIndexOfSide(Side side) { return (side == Side::left) ? getEqualsSignIndex() - 1 : formula.size(); };
    Side getIdentifierSide(char identifier);
    std::string_view getSideView(Side side);
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
      //assert((variableCounter > 0) && "FORMULA MUST HAVE AT LEAST ONE IDENTIFIER";
    };
    T solveFor(char identifier);
    void addValueFor(char identifier, T value);
  };
  Equation(std::string_view, std::vector<char>) -> Equation<double>;

  template <typename T> Side Equation<T>::getIdentifierSide(char identifier) {
    return (String::containsCharacter(getLeftSideView(), identifier)) ? Side::left : Side::right;
  }

  template <typename T> std::string_view Equation<T>::getSideView(Side side) {
    return (side == Side::left) ? getLeftSideView() : getRightSideView();
  }

  template <typename T> std::string_view Equation<T>::getLeftSideView() {
    std::string_view leftOperand{formula};
    leftOperand.remove_suffix(formula.size() - getEqualsSignIndex());
    return leftOperand;
  }

  template <typename T> std::string_view Equation<T>::getRightSideView() {
    std::string_view leftOperand{formula};
    leftOperand.remove_prefix(getEqualsSignIndex() + 1);
    return leftOperand;
  }

  template <typename T> T Equation<T>::solveFor(char identifier) {
    //assert( (formula.find(identifier) != std::string::npos) && "IDENTIFIER DOES NOT EXIST");
    //assert( (variableCounter == 1) && "THERE CANNOT BE MORE THAN ONE UNKNOWN VARIABLE IN THE FORMULA");
    Side identifierSide { getIdentifierSide(identifier) };
    if (getSideView(identifierSide).size() == 1) {
      Operation<T> result { getSideView(getOppositeSide(identifierSide)) };
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
    Side identifierSide { getIdentifierSide(identifier) };
    Side oppositeSide { getOppositeSide(identifierSide) };
    //assuming there's no parenthesis and that the operator and operands are to the right
    //place parentheses at the right side of the equal sign
    String::addToString(formula, "(", getFirstIndexOfSide(oppositeSide));
    String::addToString(formula, ")", getLastIndexOfSide(oppositeSide));
    //change the operator and pass it to the right side with the number
    for (size_t i{0}; true; ++i) {
      if (formula[i] == identifier) {
        formula += getOpposite(formula[i + 1]);
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
