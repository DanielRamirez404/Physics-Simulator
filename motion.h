#pragma once
#include "variable.h"
#include "distances.h"
#include <string_view>
#include <array>
#include <vector>

namespace MotionIdentifiers {
  inline constexpr char acceleration {'a'};
  inline constexpr char initialVelocity {'o'};
  inline constexpr char finalVelocity {'V'};
  inline constexpr char distance {'d'};
  inline constexpr char time {'t'};
  inline constexpr std::array<char, 5> identifiersList { acceleration, initialVelocity, finalVelocity, distance, time };
}

class Motion {
private:
  DistancesToPrint distances{};
  std::array<Variable, 5> variables {{ 
    Variable {MotionIdentifiers::acceleration}, 
    Variable {MotionIdentifiers::initialVelocity},
    Variable {MotionIdentifiers::finalVelocity},
    Variable {MotionIdentifiers::distance},
    Variable {MotionIdentifiers::time} }};
  Variable& getVariable(char identifier);
  std::string_view getFormulaFor(char identifier);
  std::vector<char> getVariablesFor(char identifier);
  int countUnknownVariables(std::string_view formula);
  void printCurrentState(float time);
  void printlInDistance(std::string_view string, unsigned int distance);
  float getCurrentDistance(float currentTime);
  void determineVariable(char identifier);
public:
  Motion() {
    getVariable(MotionIdentifiers::time).setSignedness(false);
  };
  int countSetVariables();
  bool areAllVariablesSet();
  bool canDetermineRemainingVariables();
  void determineRemainingVariables();
  bool canSimulate();
  void simulate();
  void setVariable(char identifier, float value);
  bool isVariableSet(char identifier);
  float getVariableValue(char identifier);
};
