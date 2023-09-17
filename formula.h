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

bool isNumber(char myChar);
bool isNumeric(char myChar);
bool isOperator(char myChar);
bool isParenthesis(char myChar);
bool isMathSymbol(char myChar);
bool isMathRelated(char myChar);
int getOperatorPriority(char myOperator);
char getOppositeOperator(char myOperator);
bool isMinPriority(char myChar);
bool isMidPriority(char myChar);
bool isMaxPriority(char myChar);
inline constexpr int noOperatorPriority{0};
inline constexpr int minOperatorPriority{1};
inline constexpr int midOperatorPriority{2};
inline constexpr int maxOperatorPriority{3};

bool isNumberDecimal(std::string_view numberString);

namespace Math {
  //void writeParenthesisByPriority(std::string& formula);
  void addParenthesisAroundOperator(std::string& formula, size_t operatorIndex);
}
