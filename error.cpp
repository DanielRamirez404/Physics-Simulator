#include "error.h"
#include <string_view>
#include <iostream>

void Error::add(std::string_view errorMessage) {
  message = errorMessage;
  hasBeenFound = true;
}

void Error::assert() {
  if (hasBeenFound) {
    std::cerr << "Error found: " << message << '\n';
    std::abort();
  }
}
