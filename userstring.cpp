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
