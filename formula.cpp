#include "formula.h"
#include "userstring.h"
#include <cassert>
#include <cstddef>
#include <string>
#include <string_view>

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

bool Math::Formula::isMinusSign(std::string_view formula, size_t index) {
  bool isRightChar { formula[index] == '-' };
  bool comesAfterNumber { (index == 0) ? false : isNumber(formula[index - 1]) };
  bool comesBeforeNumber { (index < formula.size() - 1) ? true : isNumber(formula[index + 1]) };
  return isRightChar && !comesAfterNumber && comesBeforeNumber;
}

bool Math::Formula::isPartOfNumber(std::string_view formula, size_t index) {
  return isNumeric(formula[index]) || isMinusSign(formula, index);
}

bool Math::Formula::isNumberDecimal(std::string_view numberString) {
  int pointCounter{0};
  for (size_t i{0}; i < numberString.size(); ++i) {
    if (numberString[i] == '.') {
    assert((pointCounter <= 1) && "NUMBERS CAN\'T HAVE MORE THAN ONE DECIMAL POINT");
    assert(isNumber(numberString[i - 1]) && isNumber(numberString[i + 1]) && "DECIMAL POINTS MUST BE SURROUNDED BY NUMBERS");
    ++pointCounter;
    }
  }
  return true;
}

bool Math::Formula::areThereParenthesis(std::string_view formula) {
  for (size_t i{0}; i < formula.size(); ++i) {
    if (isParenthesis(formula[i])) return true;
  }
  return false;
}

void Math::Formula::assertParenthesisValidation(std::string_view formula) {
  int openCounter{0};
  int closeCounter{0};
  for (size_t i{0}; i < formula.size(); ++i) {
    if (isParenthesis(formula[i])) {
      (formula[i] == '(') ? ++openCounter : ++closeCounter;
      assert((closeCounter <= openCounter) && "CAN\'T CLOSE PARENTHESIS WITHOUT OPENING");
    }
  }
  assert((openCounter == closeCounter) && "NUMBER OF OPEN AND CLOSE PARENTHESIS MUST MATCH");
}

size_t Math::Formula::getFirstParenthesisOpeningIndex(std::string_view formula) {
  assert(areThereParenthesis(formula) && "THERE IS NOT ANY PARENTHESIS TO GET INDEX FROM");
  size_t firstParenthesisOpenIndex{};
  for (size_t i{0}; i < formula.size(); ++i) {
    if (formula[i] == '(') {
      firstParenthesisOpenIndex = i;
      break;
    }
  }
  return firstParenthesisOpenIndex;
}

size_t Math::Formula::getFirstParenthesisClosingIndex(std::string_view formula) {
  assert(areThereParenthesis(formula) && "THERE IS NOT ANY PARENTHESIS TO GET INDEX FROM");
  size_t firstParenthesisCloseIndex{};
  int parenthesDeepness{0}; // increases everytime a new parenthesis is open and decreases if it closes
  for (size_t i{0}; i < formula.size(); ++i) {
    if (isParenthesis(formula[i])) {
      if ((formula[i] == ')') && (parenthesDeepness == 1)) {
        firstParenthesisCloseIndex = i;
        break;
      }
      (formula[i] == '(') ? ++parenthesDeepness : --parenthesDeepness;
    }
  }
  return firstParenthesisCloseIndex;
}

void Math::Formula::removeWhitespaces(std::string& formula) {
  String::eraseWhitespaces(formula);
}

bool Math::Formula::areCharactersValid(std::string_view formula) {
  for (size_t i{0}; i < formula.size(); ++i) {
    if (!isMathRelated(formula[i])) return false;
  }
  return true;
}
