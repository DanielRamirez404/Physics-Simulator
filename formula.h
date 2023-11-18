#pragma once
#include "operator priorities.h"
#include "userstring.h"
#include "side.h"
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
    bool areCharactersValid() const;
    bool isThereAnyBadlyPlacedOperator() const;
    bool areThereMinPriorityOperator() const;
    size_t findAnyOperator() const;
  public:
    Formula() = default;
    explicit Formula(std::string_view myFormula) : String{myFormula} { format(); };
    Formula(std::string_view myFormula, const std::vector<char>& myVariables) : String{myFormula}, variables{myVariables} { format(); };
    void setFormula(std::string_view myFormula);
    void setFormula(std::string_view myFormula, const std::vector<char>& myVariables);
    void addValueFor(char identifier, std::string_view value);
    void assertIsValid() const;
    void writeParenthesesAtMaxPriority();
    void addParentheses();
    bool comesBeforeNumber(size_t index) const;
    bool comesAfterNumber(size_t index) const;
    bool isConsecutiveMinusSign(size_t index) const;
    bool isMinusSign(size_t index) const;             // instead of substraction operator
    bool isTrueOperator(size_t index) const;          // does not include minus signs
    bool isPartOfNumber(size_t index) const;          // includes minus signs
    bool isWrappedUpByParentheses(size_t index) const;
    bool isWrappedUpByParentheses() const;
    int getNumberofOperations() const;
    Operators::Priority getMaxOperatorPriority() const;
    int getParenthesisDeepness(size_t index) const;
    size_t getFirstParenthesisOpeningIndex() const;   // returns 0 if there's no parenthesis
    size_t getFirstParenthesisClosingIndex() const;   // returns 0 if there's no parenthesis
    size_t getFirstWrappingParenthesisOpeningIndex(size_t index) const;
    size_t getFirstWrappingParenthesisClosingIndex(size_t index) const;
    std::string getAdjacentNumberString(size_t index, Side numberSide);
    std::string cutAdjacentNumberString(size_t index, Side numberSide);
  };
}
