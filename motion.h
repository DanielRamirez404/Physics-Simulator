#pragma once
#include "variable.h"
#include <string_view>
#include <array>
#include <vector>

namespace MotionVariables {
  inline constexpr std::array<char, 5> list { 'a', 'o', 'V', 'd', 't' };
}

class Motion {
private:
  std::array<Variable, 5>  variables {{ 'a', 'o', 'V', 'd', 't' }};
  Variable& getVariable(char identifier);
  std::string_view getFormulaFor(char identifier);
  std::vector<char> getVariablesFor(char identifier);
  int countUnknownVariables(std::string_view formula);
  void printCurrentState(float time);
  float getCurrentDistance(float currentTime);
  void determineVariable(char identifier);
public:
  int countSetVariables();
  bool areAllVariablesSet();
  bool canDetermineRemainingVariables();
  void determineRemainingVariables();
  void simulate();
  void setVariable(char identifier, float value);
  bool isVariableSet(char identifier);
  float getVariableValue(char identifier);
};
