#pragma once
#include "operation.h"
#include "formula.h"
#include "math characters.h"
#include "error.h"
#include <cstddef>
#include <string>
#include <string_view>
#include <vector>
#include <iterator>
#include <algorithm>

namespace Math {

  enum class Side {
    left,
    right,
  };

  Side getOppositeSide(Side side) {
    return (side == Side::left) ? Side::right : Side::left;
  }

  template <typename T> class Equation {
  private:
    Formula leftSideFormula{};
    Formula rightSideFormula{};
    Formula* variableFormula{};
    Formula* nonVariableFormula{};
    std::vector<char> variables{};
    void assignFormulas(std::string_view formula, const std::vector<char>& myVariableNames);
    void identifyFormulasFor(char identifier);
    bool areVariablesValid();
    Side getOperatorSide(char identifier);
    size_t findOutermost(char identifier, Side indexSide);
    size_t findOperator(char identifier, Side operatorSide);
    void simplifyVariableFormulaParentheses();
    std::string cutNumberString(Side operatorSide, size_t operatorIndex);
    std::string cutParenthesesString(Side operatorSide, size_t operatorIndex);
    void moveSingleOperation(char identifier);
    void moveMinPriorityOperator(char myOperator, Side operatorSide, size_t operatorIndex, std::string_view numberString);
    void moveMidPriorityOperator(char myOperator, Side operatorSide, size_t operatorIndex, std::string_view numberString);
    void moveMaxPriorityOperator(char myOperator, Side operatorSide, size_t operatorIndex, std::string_view numberString);
  public:
    Equation(const Equation&) = delete;
    Equation& operator=(const Equation&) = delete; 
    Equation(std::string_view formula, const std::vector<char>& myVariableNames) : variables(myVariableNames) {
      assertWithMessage(std::count(formula.begin(), formula.end(), '=') == 1, "THERE MUST BE ONE (AND ONLY ONE) EQUALS SIGN IN THE FORMULA");
      assignFormulas(formula, myVariableNames);
      assertWithMessage(areVariablesValid(), "THE VARIABLES IN THE FORMULA ARE NOT VALID. REMEMBER THEY MUST BE USED ONCE (AND ONLY ONCE)");
    };
    T solveFor(char identifier);
    void addValueFor(char identifier, T value);
  };
  Equation(std::string_view, std::vector<char>) -> Equation<double>;
}

template <typename T> void Math::Equation<T>::assignFormulas(std::string_view formula, const std::vector<char>& myVariableNames) {
  const size_t equalsSignIndex { formula.find('=') };
  leftSideFormula.setFormula(formula.substr(0, equalsSignIndex), myVariableNames);
  rightSideFormula.setFormula(formula.substr(equalsSignIndex + 1, formula.size() - equalsSignIndex), myVariableNames);
  leftSideFormula.assertIsValid();
  rightSideFormula.assertIsValid();
}

template <typename T> void Math::Equation<T>::identifyFormulasFor(char identifier) {
  const Side identifierSide { (leftSideFormula.contains(identifier)) ? Side::left : Side::right };
  variableFormula = (identifierSide == Side::left) ? &leftSideFormula : &rightSideFormula;
  nonVariableFormula = (identifierSide == Side::right) ? &leftSideFormula : &rightSideFormula;
}

template <typename T> bool Math::Equation<T>::areVariablesValid() {
  return std::all_of(variables.begin(), variables.end(), [&](char identifier) { 
    identifyFormulasFor(identifier);
    return (variableFormula->count(identifier) == 1) && !nonVariableFormula->contains(identifier);
  } );
}

template <typename T> size_t Math::Equation<T>::findOutermost(char identifier, Side indexSide) {
  size_t index { variableFormula->find(identifier) };
  if (variableFormula->isWrappedUpByParentheses(index)) {
    index = (indexSide == Side::left) ? variableFormula->getFirstWrappingParenthesisOpeningIndex(index) : variableFormula->getFirstWrappingParenthesisClosingIndex(index);
  }
  return index;
}

template <typename T> T Math::Equation<T>::solveFor(char identifier) {
  assert((variables.size() == 1) && "THERE CANNOT BE MORE THAN ONE UNKNOWN VARIABLE IN THE FORMULA");
  identifyFormulasFor(identifier);
  T result{};
  while (variableFormula->size() > 2) moveSingleOperation(identifier);
  Operation<T> myOperation{ nonVariableFormula->get() };
  if (variableFormula->size() == 1) result = myOperation.solve();
  if (variableFormula->size() == 2 && variableFormula->at(0) == '-') result = -myOperation.solve();
  return result;
}

template <typename T> void Math::Equation<T>::addValueFor(char identifier, T value) {
  identifyFormulasFor(identifier);
  variableFormula->addValueFor(identifier, std::to_string(value));
  variables.erase(std::find(variables.begin(), variables.end(), identifier));
}

template <typename T> void Math::Equation<T>::moveSingleOperation(char identifier) {
  nonVariableFormula->addParentheses();
  simplifyVariableFormulaParentheses();
  const Side operatorSide { getOperatorSide(identifier) };
  const std::string numberString { cutNumberString(operatorSide, findOperator(identifier, operatorSide)) };
  const size_t operatorIndex{ findOperator(identifier, operatorSide) };
  const char myOperator { variableFormula->at(operatorIndex) };
  switch (Operators::getPriority(myOperator)) {
    case Operators::Constants::minOperatorPriority:
      moveMinPriorityOperator(myOperator, operatorSide, operatorIndex, numberString);
      break;
    case Operators::Constants::midOperatorPriority:
      moveMidPriorityOperator(myOperator, operatorSide, operatorIndex, numberString);
      break;
    case Operators::Constants::maxOperatorPriority:
      moveMaxPriorityOperator(myOperator, operatorSide, operatorIndex, numberString);
      break;
  }
}

template <typename T> size_t Math::Equation<T>::findOperator(char identifier, Side operatorSide) {
  const size_t outermostVariableIndex { findOutermost(identifier, operatorSide) };
  return (operatorSide == Side::right) ? outermostVariableIndex + 1 : outermostVariableIndex - 1 ;
}

template <typename T> void Math::Equation<T>::simplifyVariableFormulaParentheses() {
  if ( variableFormula->isWrappedUpByParentheses() ) {
    variableFormula->erase(0);
    variableFormula->erase(variableFormula->size() - 1);
  }
}

template <typename T> Math::Side Math::Equation<T>::getOperatorSide(char identifier) {
  const size_t outermostVariableIndex { findOutermost(identifier, Side::right) };
  return (outermostVariableIndex == variableFormula->size() - 1) ? Side::left : Side::right;
}

template <typename T> std::string Math::Equation<T>::cutNumberString(Side operatorSide, size_t operatorIndex) {
  const size_t afterOperatorIndex{ (operatorSide == Side::right) ? operatorIndex + 1 : operatorIndex - 1 };
  if (isParenthesis(variableFormula->at(afterOperatorIndex))) {
    return cutParenthesesString(operatorSide, operatorIndex);
  }
  return (operatorSide == Side::left) ? variableFormula->cutPreviousNumberString(operatorIndex) : variableFormula->cutNextNumberString(operatorIndex);
}

template <typename T> std::string Math::Equation<T>::cutParenthesesString(Side operatorSide, size_t operatorIndex) {
  size_t firstIndex{ (operatorSide == Side::right) ? operatorIndex + 1 : 0 };
  size_t lastIndex{ (operatorSide == Side::left) ? operatorIndex - 1 : 0 };
  size_t& openingIndex { (operatorSide == Side::right) ? firstIndex : lastIndex };
  size_t& closingIndex{ (operatorSide == Side::left) ? firstIndex : lastIndex };
  const char closingChar { (operatorSide == Side::right) ? ')' : '('};
  int parenthesisDeepness{0};
  auto iteratorLimit { (operatorSide == Side::left) ? variableFormula->begin() : variableFormula->end() };
  auto closingIterator { std::find_if(variableFormula->begin() + static_cast<int>(openingIndex), iteratorLimit, [&](char myChar) {
    if (!isParenthesis(myChar)) return false;
    (myChar == closingChar) ? --parenthesisDeepness : ++parenthesisDeepness;
    return (parenthesisDeepness == 0);
  } ) };
  closingIndex = static_cast<size_t>(std::distance(variableFormula->begin(), closingIterator));
  size_t totalSize {lastIndex - firstIndex}; 
  return variableFormula->cut(firstIndex, totalSize + 1);
}

template <typename T> void Math::Equation<T>::moveMinPriorityOperator(char myOperator, Side operatorSide, size_t operatorIndex, std::string_view numberString) {
  if (operatorSide == Side::left && operatorIndex == 0) {   //if there are no more left-sided operations
    nonVariableFormula->append('-');                        //this will always change the numberString signedness
    if (variableFormula->at(0) == '+') variableFormula->erase(operatorIndex);
  } else {
    nonVariableFormula->append(Operators::getOpposite(myOperator));
    if (operatorSide == Side::left) {
      myOperator = variableFormula->cut(operatorIndex - 1);
    } else {
      variableFormula->erase(operatorIndex);
    }
  }
  nonVariableFormula->append(numberString);
}

template <typename T> void Math::Equation<T>::moveMidPriorityOperator(char myOperator, Side operatorSide, size_t operatorIndex, std::string_view numberString) {
  if (operatorSide == Side::left && myOperator == '/') {
    nonVariableFormula->prepend('/');
    nonVariableFormula->prepend(numberString);
  } else {
    nonVariableFormula->append(Operators::getOpposite(myOperator));
    nonVariableFormula->append(numberString);
  }
  variableFormula->erase(operatorIndex);
}

template <typename T> void Math::Equation<T>::moveMaxPriorityOperator(char myOperator, Side operatorSide, size_t operatorIndex, std::string_view numberString) {
  assertWithMessage(operatorSide != Side::left, "CAN'T SOLVE EQUATION FOR AN INDEX");
  nonVariableFormula->append(Operators::getOpposite(myOperator));
  nonVariableFormula->append(numberString);
  variableFormula->erase(operatorIndex);
}

