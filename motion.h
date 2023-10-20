#pragma once
#include "variable.h"

class Motion {
private:
  void printCurrentState(float time);
  float getCurrentDistance(float currentTime);
  void determineAcceleration();
  void determineVelocity();
  void determineDistance();
  void determineTime();
protected:
  const int totalVariables{4};
  int setVariables{0};
  Variable acceleration{'a'}; 
  Variable velocity{'V'};
  Variable distance{'d'};
  Variable time{'t'};
public:
  bool areAllVariablesSet() { return setVariables == totalVariables; }
  bool canDetermineRemainingVariables() { return (setVariables >= 2) && !areAllVariablesSet(); };
  void determineRemainingVariables();
  void simulate();
  void setAcceleration(float myAcceleration);
  void setVelocity(float myVelocity);
  void setDistance(float myDistance);
  void setTime(float myTime);
  bool hasAccelerationBeenSet() { return acceleration.isSet(); };
  bool hasVelocityBeenSet() { return velocity.isSet(); };
  bool hasDistanceBeenSet() { return distance.isSet(); };
  bool hasTimeBeenSet() { return time.isSet(); };
  float getAcceleration() { return acceleration.get(); };
  float getVelocity() { return velocity.get(); };
  float getDistance() { return distance.get(); };
  float getTime() { return time.get(); };
};
