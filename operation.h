#pragma once
#include "formula.h"
#include "usermath.h"
#include <cstddef>
#include <cassert>
#include <string>

namespace Math {
  template <typename T> class Operation {
  private:
    std::string formula{};
    void parse();
    void solveFirstParenthesis();
    int getNumberofOperations();
    T solveForOneOperator();
    T solveByPriorities(int numberOfOperations);
    T getNumber(size_t& iterator);
    T getNumber() { size_t iterator{0}; return getNumber(iterator); };
  public:
    Operation(const char* myFormula) : formula(myFormula) { parse(); };
    Operation(std::string myFormula) : formula(myFormula) { parse(); };
    T solve();
  };
}

template <typename T> T Math::Operation<T>::solve() {
  while (Formula::areThereParenthesis(formula)) solveFirstParenthesis();
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

template <typename T> void Math::Operation<T>::parse() {
  Formula::removeWhitespaces(formula);
  assert(!formula.empty() && "OPERATIONS CAN'T BE EMPTY");
  assert(Formula::areCharactersValid(formula) && "THERE SEEMS TO BE A NON-VALID CHARACTER AS INPUT");
}

template <typename T> void Math::Operation<T>::solveFirstParenthesis() {
  Formula::assertParenthesisValidation(formula);
  constexpr int BothParenthesisCount {2};
  const size_t openingIndex{ Formula::getFirstParenthesisOpeningIndex(formula) };
  const size_t closingIndex{ Formula::getFirstParenthesisClosingIndex(formula) };
  const size_t afterOpeningIndex { openingIndex + 1 };
  const size_t afterClosingIndex { closingIndex + 1 };
  const size_t length{ afterClosingIndex - openingIndex };
  std::string parenthesisFormula{ formula.substr(afterOpeningIndex, length - BothParenthesisCount) };
  Operation internalOperation(parenthesisFormula);
  formula.replace( openingIndex, length, std::to_string( internalOperation.solve() ) );
}

template <typename T> T Math::Operation<T>::solveByPriorities(int numberOfOperations) {
  Formula::writeParenthesisByPriority(formula);
  solveFirstParenthesis();
  --numberOfOperations;
  return (numberOfOperations == 1) ? solveForOneOperator() : solveByPriorities(numberOfOperations);
}

template <typename T> T Math::Operation<T>::solveForOneOperator() {
  size_t iterator{0};
  T firstValue{ getNumber(iterator) };
  ++iterator;
  char operation{ formula[iterator] };
  ++iterator;
  T secondValue{ getNumber(iterator) };
  return doOperation<T>(firstValue, operation, secondValue);
}

template <typename T> int Math::Operation<T>::getNumberofOperations() {
  int operatorCounter{0};
  for (size_t i{0}; i < formula.size(); ++i) {
    if (isOperator(formula[i])) {
      if (Formula::isMinusSign(formula, i)) continue;
      assert((i != 0) && (i != formula.size() - 1) && "OPERATORS CAN'T EITHER START NOR END FORMULAS");
      assert(isNumber(formula[i - 1]) && Formula::isPartOfNumber(formula, i + 1) && "OPERATORS MUST BE SURROUNDED BY NUMBERS");
      ++operatorCounter;
    }
  }
  return operatorCounter;
}

template <typename T> T Math::Operation<T>::getNumber(size_t& iterator) {
  assert(Formula::isPartOfNumber(formula, iterator) && "OPERATIONS MUST START BY A NUMBER");
  const size_t firstDigit { iterator };
  while (isNumeric(formula[iterator + 1])) ++iterator;
  const size_t firstNonDigit { iterator + 1};
  const size_t totalDigits { firstNonDigit - firstDigit };
  std::string numberString{ formula.substr(firstDigit, totalDigits) };
  return static_cast<T>( (Formula::isNumberDecimal(numberString)) ? std::stod(numberString) : std::stoi(numberString) );
}
