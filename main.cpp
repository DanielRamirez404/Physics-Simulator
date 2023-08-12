#include "timed functions.h"
#include <iostream>

void testFunction() {
  static int testNumber{0};
  ++testNumber;
  std::cout << testNumber << '\n';
}

int main() {
  constexpr float seconds{2.5f};
  TimedFunction test(seconds, &testFunction);
  test.run();
  return 0;
}
