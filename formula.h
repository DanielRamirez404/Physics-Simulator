#pragma once
#include <cstddef>
#include <string>
#include <string_view>

class Error {
private:
  std::string_view message { "NO ERRORS FOUND YET" };
  bool hasBeenFound{ false };
public:
  void add(std::string_view errorMessage) { message = errorMessage; hasBeenFound = true; };
  std::string_view getMessage() { return message; };
  bool exists() { return hasBeenFound; };
};

class Formula {
private:
  Error syntaxError{};
  void removeWhitespaces();
  void checkForErrors();
  bool areCharactersValid();
public:
  std::string formula{};  //public for the moment
  Formula(std::string_view myFormula) : formula(myFormula) { 
    removeWhitespaces();
    checkForErrors();
  };
  bool isValid() { return !syntaxError.exists(); };
  std::string_view getErrorMessage() { return syntaxError.getMessage(); };
  void assertIsValid();
  bool isMinusSign(size_t index);     // instead of substraction operator
  bool isPartOfNumber(size_t index);  // includes minus signs
};

bool isNumberDecimal(std::string_view numberString);

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

namespace Math {
  bool areThereParenthesis(std::string_view formula);
  void assertParenthesisValidation(std::string_view formula);
  size_t getFirstParenthesisOpeningIndex(std::string_view formula);
  size_t getFirstParenthesisClosingIndex(std::string_view formula);

  int getMaxOperatorPriority(std::string_view formula);
  void writeParenthesisByPriority(std::string& formula);
  void addParenthesisAroundOperator(std::string& formula, size_t operatorIndex);
}
