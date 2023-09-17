#pragma once
#include "error.h"
#include <cstddef>
#include <string>
#include <string_view>

class Formula {
private:
  Error syntaxError{};
  void removeWhitespaces();
  void checkForErrors();
  bool areCharactersValid();
  bool areParenthesesValid();
  //void addParenthesisAroundOperator(size_t operatorIndex);
public:
  std::string formula{};  //public for the moment
  Formula(std::string_view myFormula) : formula(myFormula) { 
    removeWhitespaces();
    checkForErrors();
  };
  void assertIsValid() { syntaxError.assert(); };
  bool isMinusSign(size_t index);           // instead of substraction operator
  bool isTrueOperator(size_t index);        // does not include minus signs
  bool isPartOfNumber(size_t index);        // includes minus signs
  bool hasParentheses();
  size_t getFirstParenthesisOpeningIndex(); //returns 0 if there's no parenthesis
  size_t getFirstParenthesisClosingIndex(); //returns 0 if there's no parenthesis
  int getMaxOperatorPriority();
  //void writeParenthesisByPriority();
};

bool isNumberDecimal(std::string_view numberString);

namespace Math {
  //void writeParenthesisByPriority(std::string& formula);
  void addParenthesisAroundOperator(std::string& formula, size_t operatorIndex);
}
