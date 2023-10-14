#include "formula.h"
#include "math characters.h"
#include "userstring.h"
#include "uservector.h"
#include "error.h"
#include "side.h"
#include <cstddef>
#include <string_view>
#include <algorithm>

bool Math::isNumberDecimal(std::string_view numberString) { 
  return numberString.find('.') != std::string::npos;
}

void Math::Formula::setFormula(std::string_view myFormula) {
  string = myFormula;
  format(); 
}

void Math::Formula::setFormula(std::string_view myFormula, const std::vector<char>& myVariables) {
  string = myFormula;
  variables = myVariables;
  format();
}

void Math::Formula::addValueFor(char identifier, std::string_view value) {
  string.replace(string.find(identifier), 1, value);
}

void Math::Formula::format() {
  eraseWhitespaces();
  simplifyConsecutiveMinusSigns();
}

void Math::Formula::simplifyConsecutiveMinusSigns() {
  for (size_t i{1}; i < string.size(); ++i) {
    while (isConsecutiveMinusSign(i)) {
      string.erase(i - 1, 2);
      i = (i < 2) ? 0 : i - 2;
      if ((isPartOfNumber(i) || string[i] == ')') && (isPartOfNumber(i + 1) || string[i + 1] == '('))
        add('+', i + 1);
    } 
  }
}

void Math::Formula::assertIsValid() {
  Error syntaxError{};
  if (string.empty())
    syntaxError.add("FORMULA CAN\'T BE EMPTY");
  else if (!areCharactersValid())
    syntaxError.add("SEEMS LIKE THERE IS A NON-VALID CHARACTER");
  else if (std::count(string.begin(), string.end(), '(') != std::count(string.begin(), string.end(), ')'))
    syntaxError.add("NUMBER OF OPEN AND CLOSE PARENTHESES MUST MATCH");
  else if (isTrueOperator(0) || isTrueOperator(string.size() - 1))
    syntaxError.add("OPERATORS CAN'T EITHER START NOR END FORMULAS");
  else if (isThereAnyBadlyPlacedOperator())
    syntaxError.add("THERE IS A BADLY PLACED OPERATOR");
  syntaxError.assertNoFoundErrors();
}

bool Math::Formula::areCharactersValid() {
  return std::all_of(string.begin(), string.end(), [&](char myChar) { 
    return isMathRelated(myChar) || Vector::doesElementExist(variables, myChar); 
  });
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

int  Math::Formula::getParenthesisDeepness(size_t index) {
  int deepness{};
  for (size_t i{0}; i <= index; ++i) {
    if (!isParenthesis(string[i])) continue;
    (string[i] == '(') ? ++deepness : --deepness;
  }
  return deepness;
}

bool Math::Formula::isWrappedUpByParentheses(size_t index) {
  return getParenthesisDeepness(index) > 0;
}

bool Math::Formula::isWrappedUpByParentheses() {
  return (getFirstParenthesisOpeningIndex() == 0 && getFirstParenthesisClosingIndex() == size() - 1);
}

size_t Math::Formula::findAnyOperator() {
  for (size_t i{0}; i < string.size(); ++i) {
    if (isTrueOperator(i)) return i;
  }
  return 0;
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
  for (size_t i{0}; i < string.size(); ++i) {
    if (string[i] == ')' && getParenthesisDeepness(i) == 0) return i;
  }
  return 0;
}

size_t Math::Formula::getFirstWrappingParenthesisOpeningIndex(size_t index) {
  for (size_t i{index}; i > 0; --i) {
    if (string[i] == '(' && getParenthesisDeepness(i) == 1) return i;
  }
  return 0;
}

size_t Math::Formula::getFirstWrappingParenthesisClosingIndex(size_t index) {
  for (size_t i{index}; i < string.size(); ++i) {
    if (string[i] == ')' && getParenthesisDeepness(i) == 0) return i;
  }
  return 0;
}

int Math::Formula::getNumberofOperations() {
  int numberOfOperations{};
  for (size_t i{0}; i < string.size(); ++i) {
    if (!isTrueOperator(i)) continue;
    if (isWrappedUpByParentheses(i)) {
      i = getFirstWrappingParenthesisClosingIndex(i);
      continue;
    }
    ++numberOfOperations;
  }
  return numberOfOperations;
}

int Math::Formula::getMaxOperatorPriority() {
  using namespace Operators;
  if (std::none_of(string.begin(), string.end(), isParenthesis)) {
    if (std::any_of(string.begin(), string.end(), isMaxPriority)) return Constants::maxOperatorPriority;
    if (std::any_of(string.begin(), string.end(), isMidPriority)) return Constants::midOperatorPriority;
    return (areThereMinPriorityOperator()) ? Constants::minOperatorPriority : Constants::noOperatorPriority;
  }
  int maxNonParenthesisOperatorPriority{ Constants::noOperatorPriority };
  for (size_t i{0}; i < string.size(); ++i) {
    if (!isTrueOperator(i)) continue;
    if (isWrappedUpByParentheses(i)) {
      i = getFirstWrappingParenthesisClosingIndex(i);
      continue;
    }
    if (isMaxPriority(string[i])) return Constants::maxOperatorPriority;
    maxNonParenthesisOperatorPriority  = (isMidPriority(string[i])) ? Constants::midOperatorPriority : Constants::minOperatorPriority;
  }
  return maxNonParenthesisOperatorPriority;
}

bool Math::Formula::areThereMinPriorityOperator() {
  for (size_t i{0}; i < string.size(); ++i) {
    if (Operators::isMinPriority(string[i]) && !isMinusSign(i)) return true;
  }
  return false;
}

void Math::Formula::writeParenthesesAtMaxPriority() {
  int maxPriority{ getMaxOperatorPriority() };
  for (size_t i{0}; i < string.size(); ++i) {
    if (!isTrueOperator(i)) continue;
    if (isWrappedUpByParentheses(i)) {
      i = getFirstWrappingParenthesisClosingIndex(i);
      continue;
    }
    if (Operators::getPriority(string[i]) == maxPriority) {
      addParenthesesAroundOperator(i);
      return;
    }
  }
}

void Math::Formula::addParenthesesAroundOperator(size_t operatorIndex) {
  size_t rightParenthesisIndex{};
  size_t leftParenthesisIndex{};
  if ( operatorIndex + 1  == '(') {
    rightParenthesisIndex = getFirstWrappingParenthesisClosingIndex(operatorIndex + 2);
  } else {
    rightParenthesisIndex = operatorIndex + 1;
    while ((isPartOfNumber(rightParenthesisIndex))) ++rightParenthesisIndex;
  }
  if ( operatorIndex - 1  == ')') {
    leftParenthesisIndex = getFirstWrappingParenthesisOpeningIndex(operatorIndex - 2);
  } else {
    leftParenthesisIndex = operatorIndex - 1;
    while (leftParenthesisIndex > 0 && (isPartOfNumber(leftParenthesisIndex - 1))) --leftParenthesisIndex;
  }
  add(')', rightParenthesisIndex);
  add('(', leftParenthesisIndex);
}

void Math::Formula::addParentheses() {
  prepend('(');
  append(')');
}

std::string Math::Formula::getAdjacentNumberString(size_t index, Side numberSide) {
  size_t iterator { index };
  while ((numberSide == Side::right) ? isPartOfNumber(iterator + 1) : (iterator > 0 && isPartOfNumber(iterator - 1)))
    (numberSide == Side::right) ? ++iterator : --iterator;
  const size_t firstDigit { (numberSide == Side::right) ? index + 1 : iterator };
  const size_t firstNonDigit { (numberSide == Side::right) ? iterator + 1 : index };
  const size_t totalDigits { firstNonDigit - firstDigit };
  return string.substr(firstDigit, totalDigits);
}

std::string Math::Formula::cutAdjacentNumberString(size_t index, Side numberSide) {
  size_t iterator { index };
  while ((numberSide == Side::right) ? isPartOfNumber(iterator + 1) : (iterator > 0 && isPartOfNumber(iterator - 1)))
    (numberSide == Side::right) ? ++iterator : --iterator;
  const size_t firstDigit { (numberSide == Side::right) ? index + 1 : iterator };
  const size_t firstNonDigit { (numberSide == Side::right) ? iterator + 1 : index };
  const size_t totalDigits { firstNonDigit - firstDigit };
  return cut(firstDigit, totalDigits);
}
