#pragma once
#include "formula.h"
#include "usermath.h"
#include <cstddef>
#include <cassert>
#include <string>
#include <string_view>

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
    void iterate();
    int getNumberofOperations();
    T getNumber();
    void iterateThroughNumber() { while (isNumeric(formula[iterator + 1])) iterate(); };
    bool isNumberDecimal(std::string_view numberString);
  public:
    Operation(const char* myFormula) : formula(myFormula) { parse(); };
    Operation(std::string& myFormula) : formula(myFormula) { parse(); };
    T getResult() { return result; };
  };

  template <typename T> void Operation<T>::parse() {
    Formula::removeWhitespaces(formula);
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
    assert(Formula::areCharactersValid(formula) && "THERE SEEMS TO BE A NON-VALID CHARACTER AS INPUT");
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
    assert(Formula::isPartOfNumber(formula, iterator)  && "OPERATIONS MUST START BY A NUMBER");
    const size_t firstDigit { iterator };
    iterateThroughNumber();
    const size_t firstNonDigit { iterator + 1};
    const size_t totalDigits { firstNonDigit - firstDigit };
    std::string numberString{ formula.substr(firstDigit, totalDigits) };
    bool isDecimal { isNumberDecimal(numberString) };
    return static_cast<T>( (isDecimal) ? std::stod(numberString) : std::stoi(numberString) );
  }

  template <typename T> bool Operation<T>::isNumberDecimal(std::string_view numberString) {
    int pointCounter{0};
    for (size_t i{0}; i < numberString.size(); ++i) {
      if (numberString[i] == '.') {
      assert((pointCounter <= 1) && "NUMBERS CAN\'T HAVE MORE THAN ONE DECIMAL POINT");
      assert(isNumber(numberString[i - 1]) && isNumber(numberString[i + 1]) && "DECIMAL POINTS MUST BE SURROUNDED BY NUMBERS");
      ++pointCounter;
      }
    }
    return true;
  }
}
