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
    std::vector<char> variables{};
    void assignBothSidesFormulas(std::string_view formula, const std::vector<char>& myVariableNames);
    bool isIdentifierValid(char identifier);
    bool areVariablesValid();
    Side getIdentifierSide(char identifier);
    Formula& getFormulaFromSide(Side side);
    char cutOperatorFromIdentifier(char identifier, Formula& identifierFormula, Side operatorSide);
    std::string cutNumberStringFromIdentifier(char identifier, Formula& identifierFormula, Side Side);
    void moveSingleOperationToOppositeFormula(char identifier, Formula& identifierFormula, Formula& oppositeFormula);
  public:
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

template <typename T> bool Math::Equation<T>::isIdentifierValid(char identifier) {
  const Side identifierSide { getIdentifierSide(identifier) };
  Formula& identifierFormula { getFormulaFromSide(identifierSide) };
  Formula& oppositeFormula { getFormulaFromSide(getOppositeSide(identifierSide)) };
  return (identifierFormula.count(identifier) == 1) && !oppositeFormula.contains(identifier);
}

template <typename T> bool Math::Equation<T>::areVariablesValid() {
  return std::all_of(variables.begin(), variables.end(), [&](char identifier) { return isIdentifierValid(identifier); } );
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
  Formula& identifierFormula { getFormulaFromSide(identifierSide) };
  Formula& oppositeFormula { getFormulaFromSide(getOppositeSide(identifierSide)) };
  while (identifierFormula.size() > 1) moveSingleOperationToOppositeFormula(identifier, identifierFormula, oppositeFormula);
  Operation<T> result { getFormulaFromSide(getOppositeSide(identifierSide)).get() };
  return result.solve();
}

template <typename T> void Math::Equation<T>::addValueFor(char identifier, T value) {
  getFormulaFromSide(getIdentifierSide(identifier)).addValueFor(identifier, std::to_string(value));
  variables.erase(std::find(variables.begin(), variables.end(), identifier));
}

template <typename T> void Math::Equation<T>::moveSingleOperationToOppositeFormula(char identifier, Formula& identifierFormula, Formula& oppositeFormula) {
  //it's getting reworked!
  //it won't work as expected yet
  //assuming there's no parenthesis on the identifier's side
  oppositeFormula.addParentheses();
  const Side operatorSide { (identifierFormula.isTrueOperator(identifierFormula.find(identifier) - 1)) ? Side::left : Side::right };
  const char myOperator { cutOperatorFromIdentifier(identifier, identifierFormula, operatorSide) };
  const std::string numberString { cutNumberStringFromIdentifier(identifier, identifierFormula, operatorSide) };
  #if 0 //for later usage
  switch (Operators::getPriority(myOperator)) {
    case Operators::Constants::minOperatorPriority:
      moveMinPriorityOperation();
      break;
    case Operators::Constants::midOperatorPriority:
      moveMidPriorityOperation();
      break;
    case Operators::Constants::maxOperatorPriority:
      moveMaxPriorityOperation();
      break;
  }
  #endif
  oppositeFormula.add(Operators::getOpposite(myOperator), oppositeFormula.size() + 1);
  oppositeFormula.add(numberString, oppositeFormula.size() + 1);
  if (identifierFormula[identifierFormula.find(identifier) - 1] == '+' && identifierFormula.find(identifier) - 1 == 0) {
    identifierFormula.erase(identifierFormula.find(identifier) - 1, 1);
  }
}

template <typename T> char Math::Equation<T>::cutOperatorFromIdentifier(char identifier, Formula& identifierFormula, Side operatorSide) {
  size_t index { (operatorSide == Side::right) ? identifierFormula.find(identifier) + 1 : identifierFormula.find(identifier) - 1};
  return identifierFormula.cut(index);
}

template <typename T> std::string Math::Equation<T>::cutNumberStringFromIdentifier(char identifier, Formula& identifierFormula, Side Side) {
  const size_t identifierIndex{ identifierFormula.find(identifier) };
  return (Side == Side::left) ? identifierFormula.cutPreviousNumberString(identifierIndex - 1) : identifierFormula.cutNextNumberString(identifierIndex + 1);
}
