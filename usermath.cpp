#include "usermath.h"

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

bool Math::isNumeric(char myChar) {
  return (isNumber(myChar) || (myChar == '.'));
}

bool Math::isOperator(char myChar) {
  switch (myChar) {
    case '+':
    case '-':
    case '*':
    case '/':
    case '^':
      return true;
  }
  return false;
}

bool Math::isParenthesis(char myChar) {
  return ((myChar == '(') || (myChar == ')'));
}

bool Math::isMathSymbol(char myChar) {
  return (isOperator(myChar) || isParenthesis(myChar) || (myChar == '.'));
}

bool Math::isMathRelated(char myChar) {
  return (isMathSymbol(myChar) || isNumber(myChar));
}
