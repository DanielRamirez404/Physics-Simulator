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

bool Math::isNumber(char myChar) {
  switch (myChar) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      return true;
  }
  return false;
}

bool Math::isMathRelated(char myChar) {
  return (isMathSymbol(myChar) || isNumber(myChar));
}
