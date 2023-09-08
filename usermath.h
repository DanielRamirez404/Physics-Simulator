#pragma once
#include <cmath>
#include <cassert>

namespace Math {
  template <typename T> T squareOf(T number) {
    return number * number;
  }

  template <typename T> T toThePower(T base, int power) {
    if (power == 0) {
      return static_cast<T>(1);
    }
    T result{1};
    for (int i{0}; i < power; ++i) {
      result *= base;
    }
    return (power > 0) ? result : static_cast<T>(1 / result);
  }

  template <typename T> T squareRootOf(T number) {
    assert((number > 0) && "CAN\'T CALCULATE SQUARE ROOT OF NEGATIVE NUMBER");
    return static_cast<T>(sqrt(number));
  }

  template <typename T> int percentage(T relativeValue, T maxValue) {
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
        result = toThePower<T>(firstValue, static_cast<int>(secondValue));
        break;
      default:
        assert(false && "OPERATOR DOES NOT EXIST");
    }
    return result;
  }
}
