#pragma once
#include <cmath>
#include <cassert>

namespace Math {
  template <typename T> T exponentiation(T base, int power) {
    if (power == 0) {
      return static_cast<T>(1);
    }
    T result{1};
    for (int i{0}; i < power; ++i) {
      result *= base;
    }
    return (power > 0) ? result : static_cast<T>(1 / result);
  }

  template <typename T> T root(T radicand , int index) {
    assert((radicand >= 0) && "CAN\'T CALCULATE ROOT OF NEGATIVE NUMBER");
    assert((index != 0) && "CAN\'T CALCULATE 0TH ROOT");
    assert((index <= 3) && "ONLY IMPLEMENTED SQUARE AND CUBIC ROOTS");
    if (radicand == 0) return 0;
    T result{};
    switch (index) {
      case 1:
        result = radicand;
        break;        
      case 2:
        result = static_cast<T>(sqrt(radicand));
        break;
      case 3:
        result = static_cast<T>(cbrt(radicand));
        break;
    }
    return result;
  }

  template <typename T> T squareRootOf(T number) {
    assert((number > 0) && "CAN\'T CALCULATE SQUARE ROOT OF NEGATIVE NUMBER");
    return static_cast<T>(sqrt(number));
  }

  template <typename T> T percentage(T relativeValue, T maxValue) {
    return ((relativeValue / maxValue) * 100);
  }

  template <typename T> T doOperation(T firstValue, char operation, T secondValue) {
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
        result = exponentiation<T>(firstValue, static_cast<int>(secondValue));
        break;
      case 'v':   //using v for roots to keep it similar to the ^ operator
        result = root(firstValue, static_cast<int>(secondValue));
        break;
      default:
        assert(false && "OPERATOR DOES NOT EXIST");
    }
    return result;
  }
  
  int round(float number) {
    return static_cast<int>(std::round(number));
  }
}
