#pragma once
#include <cmath>
#include <cassert>

namespace Math {
  template <typename T> T squareOf(T number) {
    return number * number;
  }

  template <typename T> T squareRootOf(T number) {
    assert((number > 0) && "CAN\'T CALCULATE SQUARE ROOT OF NEGATIVE NUMBER");
    return static_cast<T>(sqrt(number));
  }

  template <typename T> int percentage(T relativeValue, T maxValue) {
    return ((relativeValue / maxValue) * 100);
  }
}
