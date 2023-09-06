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
    bool isNumberDecimal();
    void addDigits(T& number);
    void addDecimals(T& number);
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
    T number{0};
    addDigits(number);
    if (isNumberDecimal()) {
      addDecimals(number);
    }
    return (isPositive) ? number : -number;
  }

  template <typename T> char Operation<T>::getOperator() {
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
}
  
