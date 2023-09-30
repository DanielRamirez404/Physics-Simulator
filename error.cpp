#include "error.h"
#include <string_view>
#include <iostream>

void abortIf(bool condition, std::string_view message) {
  if (!condition) {
    std::cerr << "Error found: " << message << '\n';
    std::abort();
  }
}

void Error::add(std::string_view errorMessage) {
  message = errorMessage;
  hasBeenFound = true;
}

void Error::assert() {
  abortIf(!hasBeenFound, message);
}
