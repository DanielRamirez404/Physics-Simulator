#pragma once
#include "operation.h"
#include "formula.h"
#include "usermath.h"
#include "userstring.h"
#include "math characters.h"
#include <cstddef>
#include <cassert>
#include <string>
#include <string_view>
#include <vector>
#include <algorithm>

namespace Math {

  enum class Side {
    left,
    right,
  };

  Side getOppositeSide(Side side) {
    return (side == Side::left) ? Side::right : Side::left;
  }

  template <typename T> struct Variable {
    char identifier{};
    //Side side{};
    T value{};
  };

  template <typename T> class Equation {
  private:
    Formula leftSideFormula{};
    Formula rightSideFormula{};
    std::vector<Variable<T>> variables{};
    Side getIdentifierSide(char identifier);
    Formula& getFormulaFromSide(Side side);
    //void rewriteFormulaToSolveFor(char identifier);
  public:
    Equation(std::string_view formula, const std::vector<char>& myVariableNames) {
      assert(std::count(formula.begin(), formula.end(), '=') == 1 && "THERE MUST BE ONE (AND ONLY ONE) EQUALS SIGN IN THE FORMULA");
      const size_t equalsSignIndex { formula.find('=') };
      leftSideFormula.setFormula(formula.substr(0, equalsSignIndex), myVariableNames);
      rightSideFormula.setFormula(formula.substr(equalsSignIndex + 1, formula.size() - equalsSignIndex), myVariableNames);
      leftSideFormula.assertIsValid();
      rightSideFormula.assertIsValid();
      std::for_each(myVariableNames.begin(), myVariableNames.end(), [&](char identifier) { variables.push_back( { identifier } ); });
      //checkThatVariablesExist();
    };
    T solveFor(char identifier);
    void addValueFor(char identifier, T value);
  };
  Equation(std::string_view, std::vector<char>) -> Equation<double>;
}

template <typename T> Math::Side Math::Equation<T>::getIdentifierSide(char identifier) {
  return (leftSideFormula.contains(identifier)) ? Side::left : Side::right;
}

template <typename T> Math::Formula& Math::Equation<T>::getFormulaFromSide(Side side) {
  return (side == Side::left) ? leftSideFormula : rightSideFormula;
}

template <typename T> T Math::Equation<T>::solveFor(char identifier) {
  assert((variables.size() == 1) && "THERE CANNOT BE MORE THAN ONE UNKNOWN VARIABLE IN THE FORMULA");
  Side identifierSide { getIdentifierSide(identifier) };
  if (getFormulaFromSide(identifierSide).getSize() > 1) {
    //rewriteFormulaToSolveFor(identifier);
  }
  Operation<T> result { getFormulaFromSide(getOppositeSide(identifierSide)).getView() };
  return result.solve();
}

template <typename T> void Math::Equation<T>::addValueFor(char identifier, T value) {
  getFormulaFromSide(getIdentifierSide(identifier)).addValueFor(identifier, std::to_string(value));
  //eliminateIdentifierFromVector();
}

#if 0
  //commented out for the time being
template <typename T> void Math::Equation<T>::rewriteFormulaToSolveFor(char identifier) {
  //needs rework
  const Side identifierSide { getIdentifierSide(identifier) };
  const Side oppositeSide { getOppositeSide(identifierSide) };
  //place parentheses at the correct side of the equal sign
  String::addToString(formula, '(', getFirstIndexOfSide(oppositeSide));
  String::addToString(formula, ')', getLastIndexOfSide(oppositeSide) + 1);
  //assuming there's no parenthesis and that the operator and operands are to the right of the identifier
  //change the operator and pass it to the corrrect side with the number
  char oppositeOperator { Operators::getOpposite(formula[getIdentifierIndex(identifier) + 1]) };
  formula.erase(getIdentifierIndex(identifier) + 1, 1);
  String::addToString(formula, oppositeOperator, getLastIndexOfSide(oppositeSide) + 1);
  //addapted from getNumber() function in the operation class
  size_t iterator{ getIdentifierIndex(identifier) + 1 };
  const size_t firstDigit { iterator };
  while (isNumeric(formula[iterator + 1])) ++iterator;
  const size_t firstNonDigit { iterator + 1};
  const size_t totalDigits { firstNonDigit - firstDigit };
  std::string numberString{ formula.substr(firstDigit, totalDigits) };
  //we got our number so we append and erase it
  formula.erase(firstDigit, totalDigits);
  String::addToString(formula, numberString, getLastIndexOfSide(oppositeSide) + 1);
}

#endif
