#pragma once
#include <string_view>

void assertWithMessage(bool condition, std::string_view message);

class Error {
private:
  std::string_view message { "NO ERRORS FOUND YET" };
  bool hasBeenFound{ false };
public:
  void add(std::string_view errorMessage);
  std::string_view getMessage() { return message; };
  bool exists() { return hasBeenFound; };
  void assert();
};
