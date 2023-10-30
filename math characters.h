#pragma once
#include "operator priorities.h"

namespace Math {
  bool isNumber(char myChar);
  bool isNumeric(char myChar);
  bool isOperator(char myChar);
  bool isParenthesis(char myChar);
  bool isMathSymbol(char myChar);
  bool isMathRelated(char myChar);
}

namespace Math::Operators {
  Priority getPriority(char myOperator);
  char getOpposite(char myOperator);
  bool isMinPriority(char myChar);
  bool isMidPriority(char myChar);
  bool isMaxPriority(char myChar);
}
