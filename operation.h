#pragma once
#include "usermath.h"
#include "userstring.h"
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
    char operation{};
    void parse();
    void solve();
    void iterate();
    void getData();
    T getNumber();
    char getOperator();
    bool isNumberPositive();
    bool areStringCharactersValid();
  public:
    Operation(const char* myFormula) : formula(myFormula) { parse(); };
    Operation(std::string& myFormula) : formula(myFormula) { parse(); };
    T getResult() { return result; };
  };

  template <typename T> void Operation<T>::parse() {
    String::eraseWhitespaces(formula);
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
    assert(areStringCharactersValid() && "THERE SEEMS TO BE A NON-VALID CHARACTER AS INPUT");
    firstValue = getNumber();
    iterate();
    operation = getOperator();
    iterate();
    secondValue = getNumber();
  }

  template <typename T> void Operation<T>::iterate() {
    assert(((iterator + 1) < formula.size()) && "CAN\'T KEEP ITERATING");
    ++iterator;
  }

  template <typename T> bool Operation<T>::areStringCharactersValid() {
    for (size_t i{0}; i < formula.size(); ++i) {
      if (!isMathRelated(formula[iterator])) {
        return false;
      }
    }
    return true;
  }

  template <typename T> T Operation<T>::getNumber() {
    bool isPositive{isNumberPositive()};
    assert(isNumber(formula[iterator]) && "OPERATIONS MUST START BY A NUMBER");
    const size_t firstDigitPosition { iterator };
    int decimalPointsCounter{0};
    while (isNumeric(formula[iterator + 1])) {
      iterate();
      if (formula[iterator] == '.') {
        assert(isNumber(formula[iterator + 1]) && "THERE MUST BE A NUMBER AFTER A DECIMAL POINT");
        ++decimalPointsCounter;
      }
    }
    assert((decimalPointsCounter <= 1) && "NUMBERS CAN\'T HAVE MORE THAN ONE DECIMAL POINT");
    const size_t lastDigitPosition { iterator };
    const size_t totalDigits { 1 + lastDigitPosition - firstDigitPosition };
    std::string numberString{ formula.substr(firstDigitPosition, totalDigits) };
    T number{};
    if (decimalPointsCounter == 1) {
      number = static_cast<T>(std::stod(numberString));
    } else {
      number = static_cast<T>(std::stoi(numberString));
    }
    return (isPositive) ? number : -number;
  }

  template <typename T> char Operation<T>::getOperator() {
    assert(isOperator(formula[iterator]));
    return formula[iterator];
  }

  template <typename T> bool Operation<T>::isNumberPositive() {
    if (formula[iterator] == '-') {
      assert(isNumber(formula[iterator + 1]) && "THERE MUST BE A NUMBER AFTER THE MINUS SIGN");
      iterate();
      return false;
    }
    return true;
  }
}
