#pragma once
#include <cstddef>
#include <string>
#include <string_view>

namespace String {
  void eraseWhitespaces(std::string& myString);
  void addToString(std::string& baseString, std::string_view stringToAdd, size_t index);
}
