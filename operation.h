#pragma once
#include "formula.h"
#include "usermath.h"
#include "userstring.h"
#include "math characters.h"
#include <cstddef>
#include <cassert>
#include <string>

using namespace Math;
using namespace Math::Operators;

namespace Math {
  template <typename T> class Operation {
  private:
    Formula formula{};
    void solveFirstParenthesis();
    int getNumberofOperations();
    T solveForOneOperator();
    T solveByPriorities(int numberOfOperations);
    T getNumber(size_t& iterator);
    T getNumber() { size_t iterator{0}; return getNumber(iterator); };
  public:
    Operation(std::string_view myFormula) : formula(myFormula) { formula.assertIsValid(); };
    T solve();
  };
  Operation(std::string_view) -> Operation<double>;
}

template <typename T> T Math::Operation<T>::solve() {
  while (formula.hasParentheses()) solveFirstParenthesis();
  T result{};
  int numberOfOperations{ getNumberofOperations() };
  switch (numberOfOperations) {
    case 0:
      result = getNumber();
      break;
    case 1: 
      result = solveForOneOperator();
      break;
    default:
      result = solveByPriorities(numberOfOperations);
      break;
  }
  return result;    
}

template <typename T> void Math::Operation<T>::solveFirstParenthesis() {
  constexpr int BothParenthesisCount {2};
  const size_t openingIndex{ formula.getFirstParenthesisOpeningIndex() };
  const size_t closingIndex{ formula.getFirstParenthesisClosingIndex() };
  const size_t afterOpeningIndex { openingIndex + 1 };
  const size_t afterClosingIndex { closingIndex + 1 };
  const size_t length{ afterClosingIndex - openingIndex };
  std::string parenthesisFormula{ formula.formula.substr(afterOpeningIndex, length - BothParenthesisCount) };
  Operation internalOperation(parenthesisFormula);
  formula.formula.replace( openingIndex, length, std::to_string( internalOperation.solve() ) );
}

template <typename T> T Math::Operation<T>::solveByPriorities(int numberOfOperations) {
  //writeParenthesisByPriority(formula.formula);
  solveFirstParenthesis();
  --numberOfOperations;
  return (numberOfOperations == 1) ? solveForOneOperator() : solveByPriorities(numberOfOperations);
}

template <typename T> T Math::Operation<T>::solveForOneOperator() {
  size_t iterator{0};
  T firstValue{ getNumber(iterator) };
  ++iterator;
  char operation{ formula.formula[iterator] };
  ++iterator;
  T secondValue{ getNumber(iterator) };
  return doOperation<T>(firstValue, operation, secondValue);
}

template <typename T> int Math::Operation<T>::getNumberofOperations() {
  int operatorCounter{0};
  for (size_t i{0}; i < formula.formula.size(); ++i) {
    if (formula.isTrueOperator(i)) {
      assert((i != 0) && (i != formula.formula.size() - 1) && "OPERATORS CAN'T EITHER START NOR END FORMULAS");
      assert(isNumber(formula.formula[i - 1]) && formula.isPartOfNumber(i+1) && "OPERATORS MUST BE SURROUNDED BY NUMBERS");
      ++operatorCounter;
    }
  }
  return operatorCounter;
}

template <typename T> T Math::Operation<T>::getNumber(size_t& iterator) {
  assert(formula.isPartOfNumber(iterator) && "OPERATIONS MUST START BY A NUMBER");
  const size_t firstDigit { iterator };
  while (isNumeric(formula.formula[iterator + 1])) ++iterator;
  const size_t firstNonDigit { iterator + 1};
  const size_t totalDigits { firstNonDigit - firstDigit };
  std::string numberString{ formula.formula.substr(firstDigit, totalDigits) };
  return static_cast<T>( (isNumberDecimal(numberString)) ? std::stod(numberString) : std::stoi(numberString) );
}
