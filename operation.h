#pragma once
#include "usermath.h"
#include "userstring.h"
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
    char operation{};
    void parseFormula();
    void iterate();
    bool areStringCharactersValid();
    T getNumberFromIterator();
    char getOperatorFromIterator();
    bool isNumberPositive();
    bool isNumberDecimal();
    void addDigits(T& number);
    void addDecimals(T& number);
  public:
    Operation(const char* myFormula) : formula(myFormula) {
      String::eraseWhitespaces(formula);
      parseFormula(); 
    };
    T solve();
  };

  template <typename T> void Operation<T>::parseFormula() {
    assert(areStringCharactersValid() && "THERE SEEMS TO BE A NON-VALID CHARACTER AS INPUT");
    firstValue = getNumberFromIterator();
    iterate();
    operation = getOperatorFromIterator();
    iterate();
    secondValue = getNumberFromIterator();
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

  template <typename T> T Operation<T>::getNumberFromIterator() {
    bool isPositive{isNumberPositive()};
    T number{0};
    addDigits(number);
    if (isNumberDecimal()) {
      addDecimals(number);
    }
    return (isPositive) ? number : -number;
  }

  template <typename T> char Operation<T>::getOperatorFromIterator() {
    char operation{formula[iterator]};
    assert(operation != '(' && operation != ')' && operation != '.' && !isNumber(operation));
    return operation;
  }

  template <typename T> bool Operation<T>::isNumberPositive() {
    if (formula[iterator] == '-') {
      assert(isNumber(formula[iterator + 1]) && "THERE MUST BE A NUMBER AFTER THE MINUS SIGN");
      iterate();
      return false;
    }
    return true;
  }

  template <typename T> bool Operation<T>::isNumberDecimal() {
    if (formula[iterator + 1] == '.') {
      iterate();
      assert(isNumber(formula[iterator + 1]) && "THERE MUST BE A NUMBER AFTER THE DECIMAL POINT");
      iterate();
      return true;
    }
    return false;
  }

  template <typename T> void Operation<T>::addDigits(T& number) {
    number *= 10;
    number += static_cast<T>(formula[iterator] - '0');
    if (isNumber(formula[iterator + 1])) {
      iterate();
      addDigits(number);
    }
  }

  template <typename T> void Operation<T>::addDecimals(T& number) {
    const int firstDecimalPosition{ static_cast<int>(iterator) };
    T decimals {0};
    addDigits(decimals);
    const int lastDecimalPosition{ static_cast<int>(iterator) };
    const int decimalCounter { 1 + lastDecimalPosition - firstDecimalPosition };
    decimals /= (toThePower<T>(10, (decimalCounter)));
    number += decimals;
  }

  template <typename T> T Operation<T>::solve() {
    T result{};
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
    return result;
  }
}
