#pragma once
#include <cstddef>
#include <vector>
#include <algorithm>

namespace Vector {
  template <typename T> bool doesElementExist(const std::vector<T>& vector, T element) {
    return std::any_of(vector.begin(), vector.end(), [&](T currentElement){ return currentElement == element; });
  }
}
