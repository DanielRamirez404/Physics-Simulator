#pragma once
#include "usermath.h"
#include <cstddef>
#include <cassert>
#include <cctype>
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
    bool areStringCharactersValid();
    void ignoreWhitespaces();
    T getNextNumberFromIterator();
    char getOperationFromIterator();
    bool isNumberPositive();
    bool isNumberDecimal();
    void addIntegersToNumber(T& number);
    void addDecimalsToNumber(T& number);
  public:
    Operation(const char* myFormula) : formula(myFormula) { parseFormula(); };
    T solve();
  };

  template <typename T> void Operation<T>::parseFormula() {
    assert( areStringCharactersValid() && "THERE SEEMS TO BE A NON-VALID CHARACTER AS INPUT");
    ignoreWhitespaces();
    firstValue = getNextNumberFromIterator();
    ignoreWhitespaces();
    operation = getOperationFromIterator();
    ignoreWhitespaces();
    secondValue = getNextNumberFromIterator();
  }

  template <typename T> bool Operation<T>::areStringCharactersValid() {
    for (size_t i{0}; i < formula.size(); ++i) {
      if (!isMathSymbol(formula[iterator]) && !isdigit(formula[iterator])) {
        return false;
      }
    }
    return true;
  }

  template <typename T> void Operation<T>::ignoreWhitespaces() {
    while (formula[iterator] == ' ') ++iterator;
  }

  template <typename T> T Operation<T>::getNextNumberFromIterator() {
    bool isPositive{isNumberPositive()};
    T number{0};
    addIntegersToNumber(number);
    if (isNumberDecimal()) {
      addDecimalsToNumber(number);
    }
    return (isPositive) ? number : -number;
  }

  template <typename T> char Operation<T>::getOperationFromIterator() {
    char operation{formula[iterator]};
    assert(operation != '(' && operation != ')' && operation != '.' && !isdigit(operation));
    ++iterator;
    return operation;
  }

  template <typename T> bool Operation<T>::isNumberPositive() {
    if (formula[iterator] == '-') {
      ++iterator;
      return false;
    }
    return true;
  }

  template <typename T> bool Operation<T>::isNumberDecimal() {
    if (formula[iterator] == '.') {
      assert(isdigit(formula[iterator + 1]) && "THERE MUST BE A NUMBER AFTER THE DECIMAL POINT");
      ++iterator;
      return true;
    }
    return false;
  }

  template <typename T> void Operation<T>::addIntegersToNumber(T& number) {
    for (; isdigit(formula[iterator]); ++iterator) {
      number *= 10;
      number += static_cast<T>(formula[iterator] - '0');
    }
  }

  template <typename T> void Operation<T>::addDecimalsToNumber(T& number) {
    for (int decimalCounter{1}; isdigit(formula[iterator]); ++decimalCounter, ++iterator) {
      T digit { static_cast<T>(formula[iterator] - '0') };
      T decimalCoefficient {1 / toThePower<T>(10, decimalCounter)};
      number += digit * decimalCoefficient;
    }
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
