#pragma once
#include "variable.h"
#include <string_view>
#include <array>
#include <vector>

namespace MotionIdentifiers {
  constexpr char acceleration {'a'};
  constexpr char initialVelocity {'o'};
  constexpr char finalVelocity {'V'};
  constexpr char distance {'d'};
  constexpr char time {'t'};
  inline constexpr std::array<char, 5> identifiersList { acceleration, initialVelocity, finalVelocity, distance, time };
}

class Motion {
private:
  std::array<Variable, 5>  variables {{ MotionIdentifiers::acceleration, MotionIdentifiers::initialVelocity, MotionIdentifiers::finalVelocity, MotionIdentifiers::distance, MotionIdentifiers::time }};
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
