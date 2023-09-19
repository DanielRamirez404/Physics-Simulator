#include "formula.h"
#include "userstring.h"
#include "math characters.h"
#include <cassert>
#include <cstddef>
#include <string>
#include <string_view>
#include <algorithm>

bool Math::isNumberDecimal(std::string_view numberString) { 
  return String::containsCharacter(numberString, '.');
}

void Math::Formula::removeWhitespaces() {
  String::eraseWhitespaces(formula);
}

void Math::Formula::checkForErrors() {
  if (formula.empty())
    syntaxError.add("FORMULA CAN\'T BE EMPTY");
  else if (!areCharactersValid())
    syntaxError.add("SEEMS LIKE THERE IS A NON-VALID CHARACTER");
  else if (hasParentheses())
    if (!areParenthesesValid()) return;
}

bool Math::Formula::areCharactersValid() { 
  return std::all_of(formula.begin(), formula.end(), isMathRelated);
}

bool Math::Formula::hasParentheses() {
  return std::any_of(formula.begin(), formula.end(), isParenthesis);
}

bool Math::Formula::areParenthesesValid() {
  const int openingCounter{ static_cast<int>(std::count(formula.begin(), formula.end(), '(')) };
  const int closingCounter{ static_cast<int>(std::count(formula.begin(), formula.end(), ')')) };
  if (openingCounter != closingCounter) {
    syntaxError.add("NUMBER OF OPEN AND CLOSE PARENTHESES MUST MATCH");
    return false;
  }
  return true;
}

bool Math::Formula::isMinusSign(size_t index) {
  if (formula[index] != '-' || index >= formula.size() - 1) return false;
  const bool comesBeforeNumber { isNumber(formula[index + 1]) };
  const bool comesAfterNumber { isNumber(formula[index - 1]) };
  return !comesAfterNumber && (comesBeforeNumber || (index == 0));
}

bool Math::Formula::isSubstractionOperator(size_t index) {
  return formula[index] == '-' && !isMinusSign(index);
}

bool Math::Formula::isTrueOperator(size_t index) {
  return isOperator(formula[index]) && !isMinusSign(index);
}

bool Math::Formula::isPartOfNumber(size_t index) {
  return isNumeric(formula[index]) || isMinusSign(index);
}

size_t Math::Formula::getFirstParenthesisOpeningIndex() {
  return String::findIndexOfCharacter(formula, '(');
}

size_t Math::Formula::getFirstParenthesisClosingIndex() {
  size_t firstClosing{};
  size_t parenthesisDeepness{};
  for (size_t i{0}; i < formula.size(); ++i) {
    if (!isParenthesis(formula[i])) continue;
    (formula[i] == '(') ? ++parenthesisDeepness : --parenthesisDeepness;
    if (formula[i] == ')' && parenthesisDeepness == 0) {
      firstClosing = i;
      break;
    }
  }
  return firstClosing;
}

int Math::Formula::getMaxOperatorPriority() {
  using namespace Operators;
  if (std::any_of(formula.begin(), formula.end(), isMaxPriority)) return Constants::maxOperatorPriority;
  if (std::any_of(formula.begin(), formula.end(), isMidPriority)) return Constants::midOperatorPriority;
  return (areThereMinPriorityOperator()) ? Constants::minOperatorPriority : Constants::noOperatorPriority;
}

bool Math::Formula::areThereMinPriorityOperator() {
  for (size_t i; i < formula.size(); ++i) {
    if (Operators::isMinPriority(formula[i]) && !isMinusSign(i)) return true;
  }
  return false;
}

void Math::Formula::writeParenthesesAtMaxPriority() {
  int maxPriority{ getMaxOperatorPriority() };
  for (size_t i{0}; i < formula.size(); ++i) {
    if (isTrueOperator(i) && (Operators::getPriority(formula[i]) == maxPriority)) {
      addParenthesesAroundOperator(i);
      break;
    }
  }
}

void Math::Formula::addParenthesesAroundOperator(size_t operatorIndex) {
  size_t leftIterator{operatorIndex - 1};
  while (isNumeric(formula[leftIterator]) && (leftIterator > 0)) --leftIterator;
  String::addToString(formula, "(", leftIterator + 1);
  ++operatorIndex;
  size_t rightIterator{operatorIndex + 1};
  while (isNumeric(formula[rightIterator]) && (rightIterator < formula.size())) ++rightIterator;
  String::addToString(formula, ")", rightIterator);
}
