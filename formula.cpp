#include "formula.h"
#include "userstring.h"
#include <cassert>
#include <cstddef>
#include <string>
#include <string_view>
#include <algorithm>

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
  switch (myOperator) {
    case '+':
    case '-':
      return minOperatorPriority;
    case '*':
    case '/':
      return midOperatorPriority;
    case '^':
    case 'v':
      return maxOperatorPriority;
  }
  return noOperatorPriority;
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

bool isMinPriority(char myChar) {
  return getOperatorPriority(myChar) == minOperatorPriority;
}

bool isMidPriority(char myChar) {
  return getOperatorPriority(myChar) == midOperatorPriority;
}

bool isMaxPriority(char myChar) {
  return getOperatorPriority(myChar) == maxOperatorPriority;
}

bool isNumberDecimal(std::string_view numberString) { 
  return String::containsCharacter(numberString, '.'); 
}

/* void Math::writeParenthesisByPriority(std::string& formula) {
  int maxOrder{ getMaxOperatorPriority(formula) };
  Formula myFormula { formula };
  for (size_t i{0}; i < formula.size(); ++i) {
    if (isOperator(formula[i]) && (getOperatorPriority(formula[i]) == maxOrder)) {
      if (myFormula.isMinusSign(i)) continue;
      addParenthesisAroundOperator(formula, i);
      break;
    }
  }
} */

void Math::addParenthesisAroundOperator(std::string& formula, size_t operatorIndex) {
  //assert(isOperator(formula[operatorIndex]) && "INDEX DOES NOT BELONG TO OPERATOR");
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

void Formula::removeWhitespaces() {
  String::eraseWhitespaces(formula);
}

void Formula::checkForErrors() {
  if (formula.empty())
    syntaxError.add("FORMULA CAN\'T BE EMPTY");
  else if (!areCharactersValid())
    syntaxError.add("SEEMS LIKE THERE IS A NON-VALID CHARACTER");
  else if (hasParentheses())
    if (!areParenthesesValid()) return;
}

bool Formula::areCharactersValid() { 
  return std::all_of(formula.begin(), formula.end(), isMathRelated);
}

bool Formula::hasParentheses() {
  return std::any_of(formula.begin(), formula.end(), isParenthesis);
}

bool Formula::areParenthesesValid() {
  const int openingCounter{ static_cast<int>(std::count(formula.begin(), formula.end(), '(')) };
  const int closingCounter{ static_cast<int>(std::count(formula.begin(), formula.end(), ')')) };
  if (openingCounter != closingCounter) {
    syntaxError.add("NUMBER OF OPEN AND CLOSE PARENTHESES MUST MATCH");
    return false;
  }
  return true;
}

bool Formula::isMinusSign(size_t index) {
  if (formula[index] != '-' || index >= formula.size() - 1) return false;
  const bool comesBeforeNumber { isNumber(formula[index + 1]) };
  const bool comesAfterNumber { isNumber(formula[index - 1]) };
  return !comesAfterNumber && (comesBeforeNumber || (index == 0));
}

bool Formula::isTrueOperator(size_t index) {
  return isOperator(formula[index]) && !isMinusSign(index);
}

bool Formula::isPartOfNumber(size_t index) {
  return isNumeric(formula[index]) || isMinusSign(index);
}

size_t Formula::getFirstParenthesisOpeningIndex() {
  return String::findIndexOfCharacter(formula, '(');
}

size_t Formula::getFirstParenthesisClosingIndex() {
  size_t firstParenthesisCloseIndex{};
  int deepness{0}; // increases everytime a new parenthesis is open and decreases if it closes
  for (size_t i{0}; i < formula.size(); ++i) {
    if (isParenthesis(formula[i])) {
      if ((formula[i] == ')') && (deepness == 1)) {
        firstParenthesisCloseIndex = i;
        break;
      }
      (formula[i] == '(') ? ++deepness : --deepness;
    }
  }
  return firstParenthesisCloseIndex;
}

int Formula::getMaxOperatorPriority() {
  if (std::any_of(formula.begin(), formula.end(), isMaxPriority)) return maxOperatorPriority;
  if (std::any_of(formula.begin(), formula.end(), isMidPriority)) return midOperatorPriority;
  if (std::any_of(formula.begin(), formula.end(), isMinPriority)) return minOperatorPriority;
  return noOperatorPriority;
} 
