#include "math characters.h"
#include "operator priorities.h"

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
  return isNumber(myChar) || (myChar == '.');
}

bool Math::isOperator(char myChar) {
  switch (myChar) {
    case '+':
    case '-':
    case '*':
    case '/':
    case '^':
    case 'v':
      return true;
  }
  return false;
}

bool Math::isParenthesis(char myChar) {
  return (myChar == '(') || (myChar == ')');
}

bool Math::isMathSymbol(char myChar) {
  return isOperator(myChar) || isParenthesis(myChar) || (myChar == '.');
}

bool Math::isMathRelated(char myChar) {
  return (isMathSymbol(myChar) || isNumber(myChar));
}

Math::Operators::Priority Math::Operators::getPriority(char myOperator) {
  switch (myOperator) {
    case '+':
    case '-':
      return Priority::low;
    case '*':
    case '/':
      return Priority::mid;
    case '^':
    case 'v':
      return Priority::max;
  }
  return Priority::none;
}

char Math::Operators::getOpposite(char myOperator) {
  switch (myOperator) {
    case '+':
      return '-';
    case '-':
      return '+';
    case '*':
      return '/';
    case '/':
      return '*';
    case '^':
      return 'v';
    case 'v':
      return '^';
  }
  return myOperator;  //returns the same char if it's not an operator
}

bool Math::Operators::isMinPriority(char myChar) {
  return getPriority(myChar) == Priority::low;
}

bool Math::Operators::isMidPriority(char myChar) {
  return getPriority(myChar) == Priority::mid;
}

bool Math::Operators::isMaxPriority(char myChar) {
  return getPriority(myChar) == Priority::max;
}
