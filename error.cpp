#include "error.h"
#include <string_view>
#include <iostream>

void assertWithMessage(bool condition, std::string_view message) {
  if (!condition) {
    std::cerr << "Error found: " << message << '\n';
    std::abort();
  }
}

void Error::add(std::string_view errorMessage) {
  message = errorMessage;
  hasBeenFound = true;
}

void Error::assertNoFoundErrors() {
  assertWithMessage(!hasBeenFound, message);
}
