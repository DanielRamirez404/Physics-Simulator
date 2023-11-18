#pragma once
#include "operation.h"
#include "formula.h"
#include "math characters.h"
#include "operator priorities.h"
#include "error.h"
#include "side.h"
#include <cstddef>
#include <string>
#include <string_view>
#include <vector>
#include <iterator>
#include <algorithm>

namespace Math {
  template <typename T> class Equation {
  private:
    Formula leftSideFormula{};
    Formula rightSideFormula{};
    Formula* variableFormula{};
    Formula* nonVariableFormula{};
    std::vector<char> variables{};
    void assignFormulas(std::string_view formula, const std::vector<char>& myVariableNames);
    void identifyFormulasFor(char identifier);
    bool isIdentifierResolvable(char identifier) const;
    Side getOperatorSide(char identifier) const;
    size_t findOutermost(char identifier, Side indexSide) const;
    size_t findOperator(char identifier, Side operatorSide) const;
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
    Equation(std::string_view formula, const std::vector<char>& myVariableNames) : variables{myVariableNames} { assignFormulas(formula, myVariableNames); };
    T solveFor(char identifier);
    void addValueFor(char identifier, T value);
  };
  Equation(std::string_view, std::vector<char>) -> Equation<double>;
}

template <typename T> void Math::Equation<T>::assignFormulas(std::string_view formula, const std::vector<char>& myVariableNames) {
  assertWithMessage(std::count(formula.begin(), formula.end(), '=') == 1, "THERE MUST BE ONE (AND ONLY ONE) EQUALS SIGN IN THE FORMULA");
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

template <typename T> bool Math::Equation<T>::isIdentifierResolvable(char identifier) const {
  return (variables.size() == 1) && (variableFormula->count(identifier) == 1) && !nonVariableFormula->contains(identifier);
}

template <typename T> size_t Math::Equation<T>::findOutermost(char identifier, Side indexSide) const {
  size_t index { variableFormula->find(identifier) };
  if (variableFormula->isWrappedUpByParentheses(index)) {
    index = (indexSide == Side::left) ? variableFormula->getFirstWrappingParenthesisOpeningIndex(index) : variableFormula->getFirstWrappingParenthesisClosingIndex(index);
  }
  return index;
}

template <typename T> T Math::Equation<T>::solveFor(char identifier) {
  identifyFormulasFor(identifier);
  assertWithMessage(isIdentifierResolvable(identifier), "THERE CANNOT BE MORE THAN ONE UNKNOWN VARIABLE IN THE FORMULA");
  simplifyVariableFormulaParentheses();
  T result{};
  while (variableFormula->getNumberofOperations() > 1 && variableFormula->getMaxOperatorPriority() > Operators::Priority::low) {
    variableFormula->writeParenthesesAtMaxPriority();
  }
  while (variableFormula->size() > 2) {
    moveSingleOperation(identifier);
    return solveFor(identifier); 
  } 
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
  const Side operatorSide { getOperatorSide(identifier) };
  const std::string numberString { cutNumberString(operatorSide, findOperator(identifier, operatorSide)) };
  const size_t operatorIndex{ findOperator(identifier, operatorSide) };
  const char myOperator { variableFormula->at(operatorIndex) };
  switch (Operators::getPriority(myOperator)) {
    case Operators::Priority::low:
      moveMinPriorityOperator(myOperator, operatorSide, operatorIndex, numberString);
      break;
    case Operators::Priority::mid:
      moveMidPriorityOperator(myOperator, operatorSide, operatorIndex, numberString);
      break;
    case Operators::Priority::max:
      moveMaxPriorityOperator(myOperator, operatorSide, operatorIndex, numberString);
      break;
    case Operators::Priority::none:
      assertWithMessage(false, "AN UNEXPECTED ERROR HAS OCURRED");
  }
}

template <typename T> size_t Math::Equation<T>::findOperator(char identifier, Side operatorSide) const {
  const size_t outermostVariableIndex { findOutermost(identifier, operatorSide) };
  return (operatorSide == Side::right) ? outermostVariableIndex + 1 : outermostVariableIndex - 1 ;
}

template <typename T> void Math::Equation<T>::simplifyVariableFormulaParentheses() {
  if ( variableFormula->isWrappedUpByParentheses() ) {
    variableFormula->erase(0);
    variableFormula->erase(variableFormula->size() - 1);
  }
}

template <typename T> Side Math::Equation<T>::getOperatorSide(char identifier) const {
  const size_t outermostVariableIndex { findOutermost(identifier, Side::right) };
  return (outermostVariableIndex == variableFormula->size() - 1) ? Side::left : Side::right;
}

template <typename T> std::string Math::Equation<T>::cutNumberString(Side operatorSide, size_t operatorIndex) {
  const size_t afterOperatorIndex{ (operatorSide == Side::right) ? operatorIndex + 1 : operatorIndex - 1 };
  if (isParenthesis(variableFormula->at(afterOperatorIndex))) {
    return cutParenthesesString(operatorSide, operatorIndex);
  }
  return variableFormula->cutAdjacentNumberString(operatorIndex, operatorSide);
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

