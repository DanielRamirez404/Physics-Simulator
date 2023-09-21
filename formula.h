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
    void checkForErrors();
    bool doParenthesesMatch();
    void addParenthesesAroundOperator(size_t operatorIndex);
    bool isMinusSign(size_t index);             // instead of substraction operator
    bool isSubstractionOperator(size_t index);
    bool areThereMinPriorityOperator();
  public:
    Formula(std::string_view myFormula) : formula(myFormula) { String::eraseWhitespaces(formula); };
    void assertIsValid() { syntaxError.assert(); };
    bool isTrueOperator(size_t index);          // does not include minus signs
    bool isPartOfNumber(size_t index);          // includes minus signs
    size_t getFirstParenthesisOpeningIndex();   //returns 0 if there's no parenthesis
    size_t getFirstParenthesisClosingIndex();   //returns 0 if there's no parenthesis
    int getMaxOperatorPriority();
    void writeParenthesesAtMaxPriority();
  };
}
