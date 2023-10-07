#pragma once
#include "operation.h"
#include "formula.h"
#include "math characters.h"
#include "error.h"
#include <cstddef>
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

  template <typename T> class Equation {
  private:
    Formula leftSideFormula{};
    Formula rightSideFormula{};
    Formula* variableFormula{};
    Formula* nonVariableFormula{};
    std::vector<char> variables{};
    void assignBothSidesFormulas(std::string_view formula, const std::vector<char>& myVariableNames);
    void identifyBothFormulasFor(char identifier);
    bool areVariablesValid();
    char getOperatorFromIdentifier(char identifier, Side operatorSide);
    std::string cutNumberStringFromIdentifier(char identifier, Side Side);
    void moveSingleOperation(char identifier);
    //void moveMinPriorityOperation(char identifier, char myOperator, Side operationSide);
    //void moveMidPriorityOperation(char identifier, char myOperator, Side operationSide);
    //void moveMaxPriorityOperation(char identifier, char myOperator, Side operationSide);
  public:
    Equation(const Equation&) = delete;
    Equation& operator=(const Equation&) = delete; 
    Equation(std::string_view formula, const std::vector<char>& myVariableNames) : variables(myVariableNames) {
      assertWithMessage(std::count(formula.begin(), formula.end(), '=') == 1, "THERE MUST BE ONE (AND ONLY ONE) EQUALS SIGN IN THE FORMULA");
      assignBothSidesFormulas(formula, myVariableNames);
      assertWithMessage(areVariablesValid(), "THE VARIABLES IN THE FORMULA ARE NOT VALID. REMEMBER THEY MUST BE USED ONCE (AND ONLY ONCE)");
    };
    T solveFor(char identifier);
    void addValueFor(char identifier, T value);
  };
  Equation(std::string_view, std::vector<char>) -> Equation<double>;
}

template <typename T> void Math::Equation<T>::assignBothSidesFormulas(std::string_view formula, const std::vector<char>& myVariableNames) {
  const size_t equalsSignIndex { formula.find('=') };
  leftSideFormula.setFormula(formula.substr(0, equalsSignIndex), myVariableNames);
  rightSideFormula.setFormula(formula.substr(equalsSignIndex + 1, formula.size() - equalsSignIndex), myVariableNames);
  leftSideFormula.assertIsValid();
  rightSideFormula.assertIsValid();
}

template <typename T> void Math::Equation<T>::identifyBothFormulasFor(char identifier) {
  const Side identifierSide { (leftSideFormula.contains(identifier)) ? Side::left : Side::right };
  variableFormula = (identifierSide == Side::left) ? &leftSideFormula : &rightSideFormula;
  nonVariableFormula = (identifierSide == Side::right) ? &leftSideFormula : &rightSideFormula;
}

template <typename T> bool Math::Equation<T>::areVariablesValid() {
  return std::all_of(variables.begin(), variables.end(), [&](char identifier) { 
    identifyBothFormulasFor(identifier);
    return (variableFormula->count(identifier) == 1) && !nonVariableFormula->contains(identifier);
  } );
}

template <typename T> T Math::Equation<T>::solveFor(char identifier) {
  assert((variables.size() == 1) && "THERE CANNOT BE MORE THAN ONE UNKNOWN VARIABLE IN THE FORMULA");
  identifyBothFormulasFor(identifier);
  while (variableFormula->size() > 1) moveSingleOperation(identifier);
  Operation<T> result { nonVariableFormula->get() };
  return result.solve();
}

template <typename T> void Math::Equation<T>::addValueFor(char identifier, T value) {
  identifyBothFormulasFor(identifier);
  variableFormula->addValueFor(identifier, std::to_string(value));
  variables.erase(std::find(variables.begin(), variables.end(), identifier));
}

template <typename T> void Math::Equation<T>::moveSingleOperation(char identifier) {
  //it's getting reworked!
  //it won't work as expected yet
  //assuming there's no parenthesis on the identifier's side
  nonVariableFormula->addParentheses();
  const Side operatorSide { (variableFormula->isTrueOperator(variableFormula->find(identifier) - 1)) ? Side::left : Side::right };
  const char myOperator { getOperatorFromIdentifier(identifier, operatorSide) };
  const std::string numberString { cutNumberStringFromIdentifier(identifier, operatorSide) };
  #if 0
  switch (Operators::getPriority(myOperator)) {
    case Operators::Constants::minOperatorPriority:
      moveMinPriorityOperation(identifier, variableFormula, nonVariableFormula, myOperator, operatorSide, numberString));
      break;
    case Operators::Constants::midOperatorPriority:
      moveMidPriorityOperation();
      break;
    case Operators::Constants::maxOperatorPriority:
      moveMaxPriorityOperation();
      break;
  }
  #endif
  nonVariableFormula->add(Operators::getOpposite(myOperator), nonVariableFormula->size() + 1);
  nonVariableFormula->add(numberString, nonVariableFormula->size() + 1);
  if ((*variableFormula)[variableFormula->find(identifier) - 1] == '+' && variableFormula->find(identifier) - 1 == 0) {
    variableFormula->erase(variableFormula->find(identifier) - 1, 1);
  }
}

template <typename T> char Math::Equation<T>::getOperatorFromIdentifier(char identifier, Side operatorSide) {
  size_t index { (operatorSide == Side::right) ? variableFormula->find(identifier) + 1 : variableFormula->find(identifier) - 1};
  return (*variableFormula)[index];
}

template <typename T> std::string Math::Equation<T>::cutNumberStringFromIdentifier(char identifier, Side Side) {
  const size_t identifierIndex{ variableFormula->find(identifier) };
  return (Side == Side::left) ? variableFormula->cutPreviousNumberString(identifierIndex - 1) : variableFormula->cutNextNumberString(identifierIndex + 1);
}

//template <typename T> void Math::Equation<T>::moveMinPriorityOperation(char identifier, char myOperator, Side operationSide, std::string_view numberString) {
//
//}
