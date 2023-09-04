#include "userstring.h"
#include <cstddef>
#include <string>

void String::eraseWhitespaces(std::string& myString) {
  for (size_t i{0}; i < myString.size(); ++i) {
    while (myString[i] == ' ') {
      myString.erase(myString.begin() + static_cast<int>(i));
    }
  }
}
