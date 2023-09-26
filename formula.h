#pragma once
#include "error.h"
#include "userstring.h"
#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace Math {
  bool isNumberDecimal(std::string_view numberString);

  class Formula {
  protected:
    std::string formula{};
    std::vector<char> variables{};
    Error syntaxError{};
    void format();
    void simplifyConsecutiveMinusSigns();
    bool areCharactersValid();
    bool areParenthesesNumbersEqual();
    void addParenthesesAroundOperator(size_t operatorIndex);
    bool isThereAnyBadlyPlacedOperator();
    bool areThereMinPriorityOperator();
  public:
    Formula() {};
    Formula(std::string_view myFormula) : formula(myFormula) { format(); };
    Formula(std::string_view myFormula, const std::vector<char>& myVariables) : formula(myFormula), variables(myVariables) { format(); };
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
    size_t getIndex(char identifier) { return formula.find(identifier); };
    bool contains(char identifier) { return String::containsCharacter(formula, identifier); };
    size_t getSize() { return formula.size(); };
    void setFormula(std::string_view myFormula) { 
      formula = myFormula;
      format(); 
    };
    void setFormula(std::string_view myFormula, const std::vector<char>& myVariables) { 
      formula = myFormula;
      variables = myVariables;
      format();
    };
    std::string_view getView() { return formula; };
    void addValueFor(char identifier, std::string_view value);
  };
}
