#include "motion.h"
#include <cmath>

void Motion::determineRemainingVariables() {
  if (!areAllVariablesSet() && canDetermineRemainingVariables()) {
    if (!acceleration.hasBeenSet) {
      determineAcceleration();
      if (areAllVariablesSet()) 
        return;
    }
    if (!velocity.hasBeenSet) {
      determineVelocity();
      if (areAllVariablesSet()) 
        return;
    }
    if (!distance.hasBeenSet) {
      determineDistance();
      if (areAllVariablesSet()) 
        return;
    }
    if (!time.hasBeenSet) {
      determineTime();
      if (areAllVariablesSet()) 
        return;
    }
  }
}

void Motion::determineAcceleration() {

}

void Motion::determineVelocity() {

}

void Motion::determineDistance() {

}

void Motion::determineTime() {

}

void Motion::setAcceleration(float myAcceleration) {
  acceleration.value = myAcceleration;
  acceleration.hasBeenSet = true;
  ++setVariables;
}

void Motion::setVelocity(float myVelocity) {
  velocity.value = myVelocity;
  velocity.hasBeenSet = true;
  ++setVariables;
}

void Motion::setDistance(float myDistance) {
  distance.value = myDistance;
  distance.hasBeenSet = true;
  ++setVariables;
}

void Motion::setTime(float myTime) {
  time.value = myTime;
  time.hasBeenSet = true;
  ++setVariables;
}
