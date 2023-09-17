#pragma once
#include <cstddef>
#include <string>
#include <string_view>

namespace String {
  void eraseWhitespaces(std::string& string);
  void addToString(std::string& baseString, std::string_view stringToAdd, size_t index);
  bool containsCharacter(std::string_view string, char myChar);
  size_t findIndexOfCharacter(std::string_view string, char myChar);
}
