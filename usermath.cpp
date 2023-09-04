#include "usermath.h"

bool Math::isMathSymbol(char myChar) {
  switch (myChar) {
    case '(':
    case ')':
    case '+':
    case '-':
    case '*':
    case '/':
    case '^':
    case '.':
      return true;
  }
  return false;
}
