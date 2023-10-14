#pragma once
#include "formula.h"
#include "userstring.h"
#include "usermath.h"
#include "math characters.h"
#include "side.h"
#include <cstddef>
#include <string>
#include <string_view>
#include <algorithm>

namespace Math {
  template <typename T> class Operation : public Formula {
  private:
    void solveFirstParenthesis();
    T solveForOneOperator();
    T solveByPriorities(int numberOfOperations);
    T getNumberFromString(const std::string& numberString);
  public:
    Operation(std::string_view myFormula) : Formula(myFormula) {};
    T solve();
  };
  Operation(std::string_view) -> Operation<double>;
}

template <typename T> T Math::Operation<T>::solve() {
  while (std::any_of(string.begin(), string.end(), isParenthesis)) solveFirstParenthesis();
  simplifyConsecutiveMinusSigns();
  T result{};
  int numberOfOperations{ getNumberofOperations() };
  switch (numberOfOperations) {
    case 0:
      result = getNumberFromString(string);
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
  const size_t openingIndex{ getFirstParenthesisOpeningIndex() };
  const size_t closingIndex{ getFirstParenthesisClosingIndex() };
  const size_t afterOpeningIndex { openingIndex + 1 };
  const size_t afterClosingIndex { closingIndex + 1 };
  const size_t length{ afterClosingIndex - openingIndex };
  Operation internalOperation(string.substr(afterOpeningIndex, length - BothParenthesisCount));
  string.replace( openingIndex, length, std::to_string( internalOperation.solve() ) );
}

template <typename T> T Math::Operation<T>::solveByPriorities(int numberOfOperations) {
  writeParenthesesAtMaxPriority();
  solveFirstParenthesis();
  --numberOfOperations;
  return (numberOfOperations == 1) ? solveForOneOperator() : solveByPriorities(numberOfOperations);
}

template <typename T> T Math::Operation<T>::solveForOneOperator() {
  size_t operatorIndex { findAnyOperator() };
  char operation{ string[operatorIndex] };
  T firstValue{ getNumberFromString(getAdjacentNumberString(operatorIndex, Side::left)) };
  T secondValue{ getNumberFromString(getAdjacentNumberString(operatorIndex, Side::right)) };
  return doOperation<T>(firstValue, operation, secondValue);
}

template <typename T> T Math::Operation<T>::getNumberFromString(const std::string& numberString) {
  return static_cast<T>( (isNumberDecimal(numberString)) ? std::stod(numberString) : std::stoi(numberString) );
}
