#include "formula.h"
#include "math characters.h"
#include "userstring.h"
#include "uservector.h"
#include <cstddef>
#include <string_view>
#include <algorithm>

bool Math::isNumberDecimal(std::string_view numberString) { 
  return numberString.find('.') != std::string::npos;
}

void Math::Formula::format() {
  eraseWhitespaces();
  simplifyConsecutiveMinusSigns();
}

void Math::Formula::simplifyConsecutiveMinusSigns() {
  for (size_t i{0}; i < string.size(); ++i) {
    while (isConsecutiveMinusSign(i)) {
      string.erase(i - 1, 2);
      i = (i == 1) ? 0 : i - 2;
    } 
  }
}

void Math::Formula::assertIsValid() {
  assertRightCharacterUsage();
  assertRightCharacterArrangement();
}

void Math::Formula::assertRightCharacterUsage() {
  if (string.empty())
    syntaxError.add("FORMULA CAN\'T BE EMPTY");
  else if (!areCharactersValid())
    syntaxError.add("SEEMS LIKE THERE IS A NON-VALID CHARACTER");
  else if (!areParenthesesNumbersEqual())
    syntaxError.add("NUMBER OF OPEN AND CLOSE PARENTHESES MUST MATCH");
  syntaxError.assert();
}

void Math::Formula::assertRightCharacterArrangement() {
  if (isTrueOperator(0) || isTrueOperator(string.size() - 1))
    syntaxError.add("OPERATORS CAN'T EITHER START NOR END FORMULAS");
  else if (isThereAnyBadlyPlacedOperator())
    syntaxError.add("THERE IS A BADLY PLACED OPERATOR");
  syntaxError.assert();
}

bool Math::Formula::areCharactersValid() {
  return std::all_of(string.begin(), string.end(), [&](char myChar) { 
    return isMathRelated(myChar) || Vector::doesElementExist(variables, myChar); 
  });
}

bool Math::Formula::areParenthesesNumbersEqual() {
  return std::count(string.begin(), string.end(), '(') == std::count(string.begin(), string.end(), ')');
}

bool Math::Formula::comesBeforeNumber(size_t index) {
  return (index < string.size() - 1) && (isPartOfNumber(index + 1) || string[index + 1] == '(');
}

bool Math::Formula::comesAfterNumber(size_t index) {
  return (index > 0) && (isPartOfNumber(index - 1) || string[index - 1] == ')');
}

bool Math::Formula::isConsecutiveMinusSign(size_t index) {
  if (index == 0) return false;
  bool existConsecutiveMinusChars{ string[index] == '-' && string[index - 1] == '-' };
  return existConsecutiveMinusChars && isNumber(string[index + 1]) && (index == 1 || !isNumber(string[index - 2]));
}

bool Math::Formula::isMinusSign(size_t index) {
  return (string[index] == '-') && !comesAfterNumber(index) && comesBeforeNumber(index);
}

bool Math::Formula::isTrueOperator(size_t index) {
  return isOperator(string[index]) && !isMinusSign(index);
}

bool Math::Formula::isPartOfNumber(size_t index) {
  return isNumeric(string[index]) || isMinusSign(index) || Vector::doesElementExist(variables, string[index]);
}

bool Math::Formula::isThereAnyBadlyPlacedOperator() {
  for (size_t i{0}; i < string.size(); ++i) {
    if (isTrueOperator(i) && !(comesAfterNumber(i) && comesBeforeNumber(i))) return true;
  }
  return false;
}

size_t Math::Formula::getFirstParenthesisOpeningIndex() {
  return find('(');
}

size_t Math::Formula::getFirstParenthesisClosingIndex() {
  size_t parenthesisDeepness{};
  for (size_t i{0}; i < string.size(); ++i) {
    if (!isParenthesis(string[i])) continue;
    (string[i] == '(') ? ++parenthesisDeepness : --parenthesisDeepness;
    if (string[i] == ')' && parenthesisDeepness == 0) return i;
  }
  return 0; //if there's no closing parentheses
}

int Math::Formula::getMaxOperatorPriority() {
  using namespace Operators;
  if (std::any_of(string.begin(), string.end(), isMaxPriority)) return Constants::maxOperatorPriority;
  if (std::any_of(string.begin(), string.end(), isMidPriority)) return Constants::midOperatorPriority;
  return (areThereMinPriorityOperator()) ? Constants::minOperatorPriority : Constants::noOperatorPriority;
}

bool Math::Formula::areThereMinPriorityOperator() {
  for (size_t i; i < string.size(); ++i) {
    if (Operators::isMinPriority(string[i]) && !isMinusSign(i)) return true;
  }
  return false;
}

void Math::Formula::writeParenthesesAtMaxPriority() {
  int maxPriority{ getMaxOperatorPriority() };
  for (size_t i{0}; i < string.size(); ++i) {
    if (isTrueOperator(i) && (Operators::getPriority(string[i]) == maxPriority)) {
      addParenthesesAroundOperator(i);
      return;
    }
  }
}

void Math::Formula::addParenthesesAroundOperator(size_t operatorIndex) {
  size_t leftIterator{operatorIndex - 1};
  while (isNumeric(string[leftIterator]) && (leftIterator > 0)) --leftIterator;
  add('(', leftIterator + 1);
  ++operatorIndex;
  size_t rightIterator{operatorIndex + 1};
  while (isNumeric(string[rightIterator]) && (rightIterator < string.size())) ++rightIterator;
  add(')', rightIterator);
}

void Math::Formula::addValueFor(char identifier, std::string_view value) {
  string.replace(string.find(identifier), 1, value);
}
