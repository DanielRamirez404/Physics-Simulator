#pragma once
#include <cmath>
#include <cctype>
#include <cassert>

namespace Math {
  bool isMathSymbol(char myChar);

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
}
