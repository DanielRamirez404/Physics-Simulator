#include "formula.h"
#include "userstring.h"
#include <cassert>
#include <cstddef>
#include <string>
#include <string_view>

bool isNumber(char myChar) {
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

bool isNumeric(char myChar) {
  return (isNumber(myChar) || (myChar == '.'));
}

bool isOperator(char myChar) {
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

bool isParenthesis(char myChar) {
  return ((myChar == '(') || (myChar == ')'));
}

bool isMathSymbol(char myChar) {
  return (isOperator(myChar) || isParenthesis(myChar) || (myChar == '.'));
}

bool isMathRelated(char myChar) {
  return (isMathSymbol(myChar) || isNumber(myChar));
}

int getOperatorPriority(char myOperator) {
  int priority{};
  switch (myOperator) {
    case '+':
    case '-':
      priority = 1;
      break;
    case '*':
    case '/':
      priority = 2;
      break;
    case '^':
    case 'v':
      priority = 3;
      break;
    default:
      assert(false && "OPERATOR DOES NOT EXIST");
  }
  return priority;
}

char getOppositeOperator(char myOperator) {
  char opposite{};
  switch (myOperator) {
    case '+':
      opposite = '-';
      break;
    case '-':
      opposite = '+';
      break;
    case '*':
      opposite = '/';
      break;
    case '^':
      opposite = 'v';
      break;
    case 'v':
      opposite = '^';
      break;
    default:
      assert(false && "OPERATOR DOES NOT EXIST");
  }
  return opposite;
}

bool Math::isMinusSign(std::string_view formula, size_t index) {
  bool isRightChar { formula[index] == '-' };
  bool comesAfterNumber { (index == 0) ? false : isNumber(formula[index - 1]) };
  bool comesBeforeNumber { (index < formula.size() - 1) ? true : (isNumber(formula[index + 1]) || formula[index + 1] == '(') };
  return isRightChar && !comesAfterNumber && comesBeforeNumber;
}

bool Math::isPartOfNumber(std::string_view formula, size_t index) {
  return isNumeric(formula[index]) || isMinusSign(formula, index);
}

bool Math::isNumberDecimal(std::string_view numberString) {
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

bool Math::areThereParenthesis(std::string_view formula) {
  for (size_t i{0}; i < formula.size(); ++i) {
    if (isParenthesis(formula[i])) return true;
  }
  return false;
}

void Math::assertParenthesisValidation(std::string_view formula) {
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

size_t Math::getFirstParenthesisOpeningIndex(std::string_view formula) {
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

size_t Math::getFirstParenthesisClosingIndex(std::string_view formula) {
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

int Math::getMaxOperatorPriority(std::string_view formula) {
  int formulaOrder{0};
  for (size_t i{0}; i < formula.size(); ++i) {
    if (isOperator(formula[i])) {
      if (isMinusSign(formula, i)) continue;
      int currentOrder { getOperatorPriority(formula[i]) };
      if (formulaOrder < currentOrder) {
        formulaOrder = currentOrder;
        if (formulaOrder == maxOperatorPriority) break;
      }
    }
  }
  return formulaOrder;
}

void Math::writeParenthesisByPriority(std::string& formula) {
  int maxOrder{ getMaxOperatorPriority(formula) };
  for (size_t i{0}; i < formula.size(); ++i) {
    if (isOperator(formula[i]) && (getOperatorPriority(formula[i]) == maxOrder)) {
      if (isMinusSign(formula, i)) continue;
      addParenthesisAroundOperator(formula, i);
      break;
    }
  }
}

void Math::addParenthesisAroundOperator(std::string& formula, size_t operatorIndex) {
  assert(isOperator(formula[operatorIndex]) && "INDEX DOES NOT BELONG TO OPERATOR");
  for (size_t i{1}; true; ++i) {
    size_t index{operatorIndex - i};
    if (!isNumeric(formula[index]) || index + 1 == 0) {
      String::addToString(formula, "(", index + 1);
      break;
    }
  }
  ++operatorIndex;
  for (size_t i{1}; true; ++i) {
    size_t index{operatorIndex + i};
    if (!isNumeric(formula[index]) || index == formula.size()) {
      String::addToString(formula, ")", index);
      break;
    }
  }
}

void Math::removeWhitespaces(std::string& formula) {
  String::eraseWhitespaces(formula);
}

void Formula::removeWhitespaces() {
  String::eraseWhitespaces(formula);
}

void Formula::checkForErrors() {
  if (formula.empty())
    syntaxError.add("FORMULA CAN\'T BE EMPTY");
  else if (!areCharactersValid())
    syntaxError.add("SEEMS LIKE THERE IS A NON-VALID CHARACTER");
}

bool Formula::areCharactersValid() {
  for (size_t i{0}; i < formula.size(); ++i) {
    if (!isMathRelated(formula[i])) return false;
  }
  return true;
}
