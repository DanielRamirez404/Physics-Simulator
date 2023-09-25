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
    T value{};
  };

  template <typename T> class Equation {
  private:
    std::string formula{};
    Formula leftSideFormula{};
    Formula rightSideFormula{};
    std::vector<Variable<T>> variables{};
    size_t getIdentifierIndex(char identifier) { return formula.find(identifier); };
    size_t getEqualsSignIndex() { return formula.find('='); };
    size_t getFirstIndexOfSide(Side side) { return (side == Side::left) ? 0 : getEqualsSignIndex() + 1; };
    size_t getLastIndexOfSide(Side side) { return (side == Side::left) ? getEqualsSignIndex() - 1 : formula.size(); };
    Side getIdentifierSide(char identifier);
    std::string_view getSideView(Side side);
    std::string_view getLeftSideView();
    std::string_view getRightSideView();
    void rewriteFormulaToSolveFor(char identifier);
  public:
    Equation(std::string_view myFormula, const std::vector<char>& myVariableNames) : formula(myFormula) 
    {
      String::eraseWhitespaces(formula);
      assert(std::count(formula.begin(), formula.end(), '=') == 1 && "THERE MUST BE ONE (AND ONLY ONE) EQUALS SIGN IN THE FORMULA");
      leftSideFormula.setFormula(getLeftSideView(), myVariableNames);
      rightSideFormula.setFormula(getRightSideView(), myVariableNames);
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
  return (String::containsCharacter(getLeftSideView(), identifier)) ? Side::left : Side::right;
}

template <typename T> std::string_view Math::Equation<T>::getSideView(Side side) {
  return (side == Side::left) ? getLeftSideView() : getRightSideView();
}

template <typename T> std::string_view Math::Equation<T>::getLeftSideView() {
  std::string_view leftOperand{formula};
  leftOperand.remove_suffix(formula.size() - getEqualsSignIndex());
  return leftOperand;
}

template <typename T> std::string_view Math::Equation<T>::getRightSideView() {
  std::string_view leftOperand{formula};
  leftOperand.remove_prefix(getEqualsSignIndex() + 1);
  return leftOperand;
}

template <typename T> T Math::Equation<T>::solveFor(char identifier) {
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

template <typename T> void Math::Equation<T>::addValueFor(char identifier, T value) {
  size_t identifierIndex { formula.find(identifier) };
  //assert( (identifierIndex != std::string::npos) && "IDENTIFIER DOES NOT EXIST");
  constexpr size_t charSize { 1 };
  formula.replace(identifierIndex, charSize, std::to_string(value));
  //eliminateIdentifierFromVector();
}

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
