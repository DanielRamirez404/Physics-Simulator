#include "userstring.h"
#include <cstddef>
#include <string>
#include <string_view>

void String::eraseWhitespaces(std::string& string) {
  for (size_t i{0}; i < string.size(); ++i) {
    while (string[i] == ' ') {
      string.erase(string.begin() + static_cast<int>(i));
    }
  }
}

void String::addToString(std::string& baseString, std::string_view stringToAdd, size_t index) {
  (index >= baseString.size()) ? baseString.append(stringToAdd) : baseString.insert(index, stringToAdd);
}

void String::addToString(std::string& baseString, char charToAdd, size_t index) {
  if (index >= baseString.size())
    baseString += charToAdd;
  else
    baseString.insert(index, 1, charToAdd);
}

bool String::containsCharacter(std::string_view string, char myChar) {
  return string.find(myChar) != std::string::npos;
}

size_t String::findIndexOfCharacter(std::string_view string, char myChar) {
  return string.find(myChar);
}
