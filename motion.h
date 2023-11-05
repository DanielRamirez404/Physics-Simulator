#pragma once
#include "variable.h"
#include <string_view>
#include <array>
#include <vector>

namespace MotionVariables::identifiers {
  constexpr char acceleration {'a'};
  constexpr char initialVelocity {'o'};
  constexpr char finalVelocity {'V'};
  constexpr char distance {'d'};
  constexpr char time {'t'};
}

namespace MotionVariables {
  inline constexpr std::array<char, 5> list { identifiers::acceleration, identifiers::initialVelocity, identifiers::finalVelocity, identifiers::distance, identifiers::time };
}

class Motion {
private:
  std::array<Variable, 5>  variables {{ MotionVariables::identifiers::acceleration, MotionVariables::identifiers::initialVelocity, MotionVariables::identifiers::finalVelocity, MotionVariables::identifiers::distance, MotionVariables::identifiers::time }};
  Variable& getVariable(char identifier);
  std::string_view getFormulaFor(char identifier);
  std::vector<char> getVariablesFor(char identifier);
  int countUnknownVariables(std::string_view formula);
  void printCurrentState(float time);
  void printlInDistance(std::string_view string, unsigned int distance);
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
