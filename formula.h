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
}

namespace Math::Formula {
  bool isMinusSign(std::string_view formula, size_t index);     // instead of substraction operator
  bool isPartOfNumber(std::string_view formula, size_t index);  // includes minus signs
  bool areThereParenthesis(std::string_view formula);
  void assertParenthesisValidation(std::string_view formula);
  void removeWhitespaces(std::string& formula);
  bool areCharactersValid(std::string_view formula);
}
