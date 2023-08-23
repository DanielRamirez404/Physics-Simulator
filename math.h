#pragma once

template <typename T> T squareOf(T number) {
  return number * number;
}

template <typename T> int percentage(T relativeValue, T maxValue) {
  return ((relativeValue / maxValue) * 100);
}
