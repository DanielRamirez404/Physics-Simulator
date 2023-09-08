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
    size_t iterator{0};
    T firstValue{};
    T secondValue{};
    T result{};
    int numberOfOperations{};
    char operation{};
    void parse();
    void getData();
    void solve();
    int getNumberofOperations();
    T getNumber();
    void iterate();
    void iterateThroughNumber() { while (isNumeric(formula[iterator + 1])) iterate(); };
  public:
    Operation(const char* myFormula) : formula(myFormula) { parse(); };
    Operation(std::string& myFormula) : formula(myFormula) { parse(); };
    T getResult() { return result; };
  };

  template <typename T> void Operation<T>::parse() {
    Formula::removeWhitespaces(formula);
    assert(!formula.empty() && "OPERATIONS CAN'T BE EMPTY");
    assert(Formula::areCharactersValid(formula) && "THERE SEEMS TO BE A NON-VALID CHARACTER AS INPUT");
    getData();
    solve();
  }

  template <typename T> void Operation<T>::solve() {
    switch (operation) {
      case '+':
        result = firstValue + secondValue;
        break;
      case '-':
        result = firstValue - secondValue;
        break;
      case '*':
        result = firstValue * secondValue;
        break;
      case '/':
        result = firstValue / secondValue;
        break;
      case '^':
        result = toThePower<T>(firstValue, static_cast<int>(secondValue));
        break;
    }
  }

  template <typename T> void Operation<T>::getData() {
    while (Formula::areThereParenthesis(formula)) {
      // works only for parenthesis of this style (c + (a + b))
      // doesn't work for style ((a + b) + (c + d)) yet
      Formula::assertParenthesisValidation(formula);
      size_t firstOpenParenthesis{};
      size_t ParenthesisLength{};
      Formula::UpdateParenthesisCoordinates(formula, firstOpenParenthesis, ParenthesisLength);
      std::string parenthesisFormula{ formula.substr(firstOpenParenthesis + 1, ParenthesisLength - 2) };
      Operation parenthesis(parenthesisFormula);
      formula.replace(firstOpenParenthesis, ParenthesisLength, std::to_string(parenthesis.getResult()));
    }
    numberOfOperations = getNumberofOperations();
    firstValue = getNumber();
    iterate();
    operation = formula[iterator];
    iterate();
    secondValue = getNumber();
  }

  template <typename T> void Operation<T>::iterate() {
    assert((iterator + 1 < formula.size()) && "CAN\'T KEEP ITERATING");
    ++iterator;
  }

  template <typename T> int Operation<T>::getNumberofOperations() {
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

  template <typename T> T Operation<T>::getNumber() {
    assert(Formula::isPartOfNumber(formula, iterator) && "OPERATIONS MUST START BY A NUMBER");
    const size_t firstDigit { iterator };
    iterateThroughNumber();
    const size_t firstNonDigit { iterator + 1};
    const size_t totalDigits { firstNonDigit - firstDigit };
    std::string numberString{ formula.substr(firstDigit, totalDigits) };
    return static_cast<T>( (Formula::isNumberDecimal(numberString)) ? std::stod(numberString) : std::stoi(numberString) );
  }
}
