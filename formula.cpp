#include "formula.h"
#include "math characters.h"
#include "userstring.h"
#include "uservector.h"
#include <cstddef>
#include <string_view>
#include <algorithm>

bool Math::isNumberDecimal(std::string_view numberString) { 
  return String::containsCharacter(numberString, '.');
}

void Math::Formula::format() {
  String::eraseWhitespaces(formula);
  simplifyConsecutiveMinusSigns();
}

void Math::Formula::simplifyConsecutiveMinusSigns() {
  for (size_t i{0}; i < formula.size(); ++i) {
    while (isConsecutiveMinusSign(i)) {
      formula.erase(i - 1, 2);
      i = (i == 1) ? 0 : i - 2;
    } 
  }
}

void Math::Formula::assertIsValid() {
  assertRightCharacterUsage();
  assertRightCharacterArrangement();
}

void Math::Formula::assertRightCharacterUsage() {
  if (formula.empty())
    syntaxError.add("FORMULA CAN\'T BE EMPTY");
  else if (!areCharactersValid())
    syntaxError.add("SEEMS LIKE THERE IS A NON-VALID CHARACTER");
  else if (!areParenthesesNumbersEqual())
    syntaxError.add("NUMBER OF OPEN AND CLOSE PARENTHESES MUST MATCH");
  syntaxError.assert();
}

void Math::Formula::assertRightCharacterArrangement() {
  if (isTrueOperator(0) || isTrueOperator(formula.size() - 1))
    syntaxError.add("OPERATORS CAN'T EITHER START NOR END FORMULAS");
  else if (isThereAnyBadlyPlacedOperator())
    syntaxError.add("THERE IS A BADLY PLACED OPERATOR");
  syntaxError.assert();
}

bool Math::Formula::areCharactersValid() {
  return std::all_of(formula.begin(), formula.end(), [&](char myChar) { 
    return isMathRelated(myChar) || Vector::doesElementExist(variables, myChar); 
  });
}

bool Math::Formula::areParenthesesNumbersEqual() {
  return std::count(formula.begin(), formula.end(), '(') == std::count(formula.begin(), formula.end(), ')');
}

bool Math::Formula::comesBeforeNumber(size_t index) {
  return (index < formula.size() - 1) && (isPartOfNumber(index + 1) || formula[index + 1] == '(');
}

bool Math::Formula::comesAfterNumber(size_t index) {
  return (index > 0) && (isPartOfNumber(index - 1) || formula[index - 1] == ')');
}

bool Math::Formula::isConsecutiveMinusSign(size_t index) {
  if (index == 0)  return false;
  bool existConsecutiveMinusChars{ formula[index] == '-' && formula[index - 1] == '-' };
  return existConsecutiveMinusChars && isNumber(formula[index + 1]) && (index == 1 || !isNumber(formula[index - 2]));
}

bool Math::Formula::isMinusSign(size_t index) {
  return (formula[index] == '-') && !comesAfterNumber(index) && comesBeforeNumber(index);
}

bool Math::Formula::isTrueOperator(size_t index) {
  return isOperator(formula[index]) && !isMinusSign(index);
}

bool Math::Formula::isPartOfNumber(size_t index) {
  return isNumeric(formula[index]) || isMinusSign(index) || Vector::doesElementExist(variables, formula[index]);
}

bool Math::Formula::isThereAnyBadlyPlacedOperator() {
  for (size_t i{0}; i < formula.size(); ++i) {
    if (isTrueOperator(i) && !(comesAfterNumber(i) && comesBeforeNumber(i))) return true;
  }
  return false;
}

size_t Math::Formula::getFirstParenthesisOpeningIndex() {
  return String::findIndexOfCharacter(formula, '(');
}

size_t Math::Formula::getFirstParenthesisClosingIndex() {
  size_t parenthesisDeepness{};
  for (size_t i{0}; i < formula.size(); ++i) {
    if (!isParenthesis(formula[i])) continue;
    (formula[i] == '(') ? ++parenthesisDeepness : --parenthesisDeepness;
    if (formula[i] == ')' && parenthesisDeepness == 0) return i;
  }
  return 0; //if there's no closing parentheses
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
      return;
    }
  }
}

void Math::Formula::addParenthesesAroundOperator(size_t operatorIndex) {
  size_t leftIterator{operatorIndex - 1};
  while (isNumeric(formula[leftIterator]) && (leftIterator > 0)) --leftIterator;
  String::addToString(formula, '(', leftIterator + 1);
  ++operatorIndex;
  size_t rightIterator{operatorIndex + 1};
  while (isNumeric(formula[rightIterator]) && (rightIterator < formula.size())) ++rightIterator;
  String::addToString(formula, ')', rightIterator);
}

void Math::Formula::addValueFor(char identifier, std::string_view value) {
  formula.replace(getIndex(identifier), 1, value);
}
