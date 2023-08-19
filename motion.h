#pragma once

struct Variable {
  float value{};
  bool hasBeenSet{false};
};

class Motion {
private:
  void determineAcceleration();
  void determineVelocity();
  void determineDistance();
  void determineTime();
protected:
  const int totalVariables{4};
  int setVariables{0};
  Variable acceleration{}; 
  Variable velocity{};
  Variable distance{};
  Variable time{};
public:
  bool areAllVariablesSet() { return setVariables == totalVariables; }
  bool canDetermineRemainingVariables() { return setVariables == 2; };
  void determineRemainingVariables();
  void setAcceleration(float myAcceleration);
  void setVelocity(float myVelocity);
  void setDistance(float myDistance);
  void setTime(float myTime);
  float getAcceleration() { return acceleration.value; };
  float getVelocity() { return velocity.value; };
  float getDistance() { return distance.value; };
  float getTime() { return time.value; };
};
