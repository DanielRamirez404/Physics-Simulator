#pragma once
#include "error.h"
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
    Error syntaxError{};
    void format();
    void simplifyConsecutiveMinusSigns();
    bool areCharactersValid();
    bool areParenthesesNumbersEqual();
    void addParenthesesAroundOperator(size_t operatorIndex);
    bool isThereAnyBadlyPlacedOperator();
    bool areThereMinPriorityOperator();
    size_t findAnyOperator();
  public:
    Formula() : String() {};
    Formula(std::string_view myFormula) : String(myFormula) { format(); };
    Formula(std::string_view myFormula, const std::vector<char>& myVariables) : String(myFormula), variables(myVariables) { format(); };
    void assertIsValid();
    void assertRightCharacterUsage();
    void assertRightCharacterArrangement();
    bool comesBeforeNumber(size_t index);
    bool comesAfterNumber(size_t index);
    bool isConsecutiveMinusSign(size_t index);
    bool isMinusSign(size_t index);             // instead of substraction operator
    bool isTrueOperator(size_t index);          // does not include minus signs
    bool isPartOfNumber(size_t index);          // includes minus signs
    size_t getFirstParenthesisOpeningIndex();   // returns 0 if there's no parenthesis
    size_t getFirstParenthesisClosingIndex();   // returns 0 if there's no parenthesis
    int getMaxOperatorPriority();
    void writeParenthesesAtMaxPriority();
    void addParentheses();
    void setFormula(std::string_view myFormula) { 
      string = myFormula;
      format(); 
    };
    void setFormula(std::string_view myFormula, const std::vector<char>& myVariables) { 
      string = myFormula;
      variables = myVariables;
      format();
    };
    void addValueFor(char identifier, std::string_view value);
    std::string getNextNumberString(size_t index);
    std::string getPreviousNumberString(size_t index);
    std::string cutNextNumberString(size_t index);
    std::string cutPreviousNumberString(size_t index);
  };
}
