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
  const int totalVariables{4};
  int setVariables{0};
  std::array<Variable, 4>  motionVariables {{ 'a', 'V', 'd', 't' }};
public:
  bool areAllVariablesSet() { return setVariables == totalVariables; }
  bool canDetermineRemainingVariables() { return (setVariables >= 2) && !areAllVariablesSet(); };
  void determineRemainingVariables();
  void simulate();
  void setVariable(char identifier, float value);
  bool isVariableSet(char identifier);
  float getVariableValue(char identifier);
};
