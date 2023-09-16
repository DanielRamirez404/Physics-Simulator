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
public:
  std::string formula{};  //public for the moment
  Formula(std::string_view myFormula) : formula(myFormula) { 
    removeWhitespaces();
    checkForErrors();
  };
  void assertIsValid() { syntaxError.assert(); };
  bool isMinusSign(size_t index);           // instead of substraction operator
  bool isPartOfNumber(size_t index);        // includes minus signs
  bool hasParentheses();
  size_t getFirstParenthesisOpeningIndex(); //returns 0 if there's no parenthesis
  size_t getFirstParenthesisClosingIndex(); //returns 0 if there's no parenthesis
};

bool isNumber(char myChar);
bool isNumeric(char myChar);
bool isOperator(char myChar);
bool isParenthesis(char myChar);
bool isMathSymbol(char myChar);
bool isMathRelated(char myChar);
int getOperatorPriority(char myOperator);
char getOppositeOperator(char myOperator);
inline constexpr int minOperatorPriority{1};
inline constexpr int maxOperatorPriority{3};

bool isNumberDecimal(std::string_view numberString);

namespace Math {
  int getMaxOperatorPriority(std::string_view formula);
  void writeParenthesisByPriority(std::string& formula);
  void addParenthesisAroundOperator(std::string& formula, size_t operatorIndex);
}
