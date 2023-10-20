#pragma once
#include "variable.h"
#include <array>

class Motion {
private:
  Variable& getVariable(char identifier);
  void printCurrentState(float time);
  float getCurrentDistance(float currentTime);
  void determineAcceleration();
  void determineVelocity();
  void determineDistance();
  void determineTime();
protected:
  std::array<Variable, 4>  variables {{ 'a', 'V', 'd', 't' }};
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
