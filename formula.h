#pragma once
#include "error.h"
#include "userstring.h"
#include <cstddef>
#include <string>
#include <string_view>

namespace Math {
  bool isNumberDecimal(std::string_view numberString);

  class Formula {
  protected:
    std::string formula{};
    Error syntaxError{};
    bool areParenthesesNumbersEqual();
    void addParenthesesAroundOperator(size_t operatorIndex);
    bool isThereAnyBadlyPlacedOperator();
    bool areThereMinPriorityOperator();
  public:
    Formula(std::string_view myFormula) : formula(myFormula) { String::eraseWhitespaces(formula); };
    void assertIsValid();
    void assertRightCharacterUsage();
    void assertRightCharacterArrangement();
    bool comesBeforeNumber(size_t index);
    bool comesAfterNumber(size_t index);
    bool isMinusSign(size_t index);             // instead of substraction operator
    bool isTrueOperator(size_t index);          // does not include minus signs
    bool isPartOfNumber(size_t index);          // includes minus signs
    size_t getFirstParenthesisOpeningIndex();   // returns 0 if there's no parenthesis
    size_t getFirstParenthesisClosingIndex();   // returns 0 if there's no parenthesis
    int getMaxOperatorPriority();
    void writeParenthesesAtMaxPriority();
  };
}
