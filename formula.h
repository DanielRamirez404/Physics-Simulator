#pragma once
#include "userstring.h"
#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace Math {
  bool isNumberDecimal(std::string_view numberString);

  class Formula : public String {
  protected:
    std::vector<char> variables{};
    void format();
    void simplifyConsecutiveMinusSigns();
    void addParenthesesAroundOperator(size_t operatorIndex);
    bool areCharactersValid();
    bool isThereAnyBadlyPlacedOperator();
    bool areThereMinPriorityOperator();
    size_t findAnyOperator();
  public:
    Formula() : String() {};
    Formula(std::string_view myFormula) : String(myFormula) { format(); };
    Formula(std::string_view myFormula, const std::vector<char>& myVariables) : String(myFormula), variables(myVariables) { format(); };
    void setFormula(std::string_view myFormula);
    void setFormula(std::string_view myFormula, const std::vector<char>& myVariables);
    void addValueFor(char identifier, std::string_view value);
    void assertIsValid();
    void writeParenthesesAtMaxPriority();
    void addParentheses();
    bool comesBeforeNumber(size_t index);
    bool comesAfterNumber(size_t index);
    bool isConsecutiveMinusSign(size_t index);
    bool isMinusSign(size_t index);             // instead of substraction operator
    bool isTrueOperator(size_t index);          // does not include minus signs
    bool isPartOfNumber(size_t index);          // includes minus signs
    bool isWrappedUpByParentheses(size_t index);
    bool isWrappedUpByParentheses();
    int getMaxOperatorPriority();
    size_t getFirstParenthesisOpeningIndex();   // returns 0 if there's no parenthesis
    size_t getFirstParenthesisClosingIndex();   // returns 0 if there's no parenthesis
    size_t getFirstWrappingParenthesisOpeningIndex(size_t index);
    size_t getFirstWrappingParenthesisClosingIndex(size_t index);
    std::string getNextNumberString(size_t index);
    std::string getPreviousNumberString(size_t index);
    std::string cutNextNumberString(size_t index);
    std::string cutPreviousNumberString(size_t index);
  };
}
