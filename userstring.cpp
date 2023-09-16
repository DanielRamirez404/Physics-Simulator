#include "userstring.h"
#include <cstddef>
#include <string>
#include <string_view>

void String::eraseWhitespaces(std::string& myString) {
  for (size_t i{0}; i < myString.size(); ++i) {
    while (myString[i] == ' ') {
      myString.erase(myString.begin() + static_cast<int>(i));
    }
  }
}

void String::addToString(std::string& baseString, std::string_view stringToAdd, size_t index) {
  (index >= baseString.size()) ? baseString.append(stringToAdd) : baseString.insert(index, stringToAdd);
}

bool String::containsCharacter(std::string_view string, char myChar) {
  return (string.find(myChar) != std::string::npos);
}

size_t String::findIndexOfCharacter(std::string_view string, char myChar) {
  return string.find(myChar);
}
