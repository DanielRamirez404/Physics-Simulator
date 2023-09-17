#pragma once

namespace Math {
  bool isNumber(char myChar);
  bool isNumeric(char myChar);
  bool isOperator(char myChar);
  bool isParenthesis(char myChar);
  bool isMathSymbol(char myChar);
  bool isMathRelated(char myChar);
}

namespace Math::Operators {
  int getPriority(char myOperator);
  char getOpposite(char myOperator);
  bool isMinPriority(char myChar);
  bool isMidPriority(char myChar);
  bool isMaxPriority(char myChar);
}

namespace Math::Operators::Constants {
  inline constexpr int noOperatorPriority{0};
  inline constexpr int minOperatorPriority{1};
  inline constexpr int midOperatorPriority{2};
  inline constexpr int maxOperatorPriority{3};
}
