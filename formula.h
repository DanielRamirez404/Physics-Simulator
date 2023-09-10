#pragma once
#include <cstddef>
#include <string>
#include <string_view>

namespace Math {
  bool isNumber(char myChar);
  bool isNumeric(char myChar);
  bool isOperator(char myChar);
  bool isParenthesis(char myChar);
  bool isMathSymbol(char myChar);
  bool isMathRelated(char myChar);
  int getOperatorPriority(char myOperator);
  inline constexpr int minOperatorPriority{1};
  inline constexpr int maxOperatorPriority{3};
}

namespace Math::Formula {
  bool isMinusSign(std::string_view formula, size_t index);     // instead of substraction operator
  bool isPartOfNumber(std::string_view formula, size_t index);  // includes minus signs
  bool isNumberDecimal(std::string_view numberString);
  bool areThereParenthesis(std::string_view formula);
  void assertParenthesisValidation(std::string_view formula);
  size_t getFirstParenthesisOpeningIndex(std::string_view formula);
  size_t getFirstParenthesisClosingIndex(std::string_view formula);
  int getMaxOperatorPriority(std::string_view formula);
  void writeParenthesisByPriority(std::string& formula);
  void addParenthesisAroundOperator(std::string& formula, size_t operatorIndex);
  void removeWhitespaces(std::string& formula);
  bool areCharactersValid(std::string_view formula);
}
