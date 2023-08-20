#include "motion.h"
#include <cmath>
#include <cassert>

void Motion::determineRemainingVariables() {
  assert (canDetermineRemainingVariables() && "CAN\'T DETERMINE REMAINING VALUES");
  if (!acceleration.hasBeenSet) {
    determineAcceleration();
    if (areAllVariablesSet()) return;
  }
  if (!velocity.hasBeenSet) {
    determineVelocity();
    if (areAllVariablesSet()) return;
  }
  if (!distance.hasBeenSet) {
    determineDistance();
    if (areAllVariablesSet()) return;
  }
  if (!time.hasBeenSet) {
    determineTime();
    if (areAllVariablesSet()) return;
  }
}

void Motion::determineAcceleration() {
  if (setVariables == 3) {
    acceleration.value = (-2 * (distance.value - (velocity.value * time.value))) / pow(time.value, 2); 
  } else if (!velocity.hasBeenSet) {
    acceleration.value = (2 * distance.value) / pow(time.value, 2);
  } else if (!distance.hasBeenSet) {
    acceleration.value = velocity.value / time.value;
  } else {
    acceleration.value = pow(velocity.value, 2) / (2 * distance.value);
  }
  acceleration.hasBeenSet = true;
  ++setVariables;
}

void Motion::determineVelocity() {
  if (setVariables == 3) {
    velocity.value = (distance.value + ((acceleration.value * pow(time.value, 2)) / 2)) / time.value;
  } else if (!acceleration.hasBeenSet) {
    velocity.value = (2 * distance.value) / time.value;
  } else if (!distance.hasBeenSet) {
    velocity.value = acceleration.value * time.value;
  } else {
    velocity.value = sqrt(2 * acceleration.value * distance.value);
  }
  velocity.hasBeenSet = true;
  ++setVariables;
}

void Motion::determineDistance() {
  if (setVariables == 3) {
    distance.value = (velocity.value * time.value) - ((acceleration.value * pow(time.value, 2)) / 2);
  } else if (!acceleration.hasBeenSet) {
    distance.value = (velocity.value * time.value) / 2;
  } else if (!velocity.hasBeenSet) {
    distance.value = (acceleration.value * pow(time.value, 2)) / 2;
  } else {
    distance.value = pow(velocity.value, 2) /  (2 * acceleration.value);
  }
  distance.hasBeenSet = true;
  ++setVariables;
}

void Motion::determineTime() {
  if (setVariables == 3) {
    time.value = sqrt(-acceleration.value * ((distance.value - (velocity.value * time.value)) / 2));
  } else if (!acceleration.hasBeenSet) {
    time.value = (2 * distance.value) / velocity.value;
  } else if (!velocity.hasBeenSet) {
    time.value = sqrt((2 * distance.value) / acceleration.value);
  } else {
    time.value = velocity.value / acceleration.value;
  }
  time.hasBeenSet = true;
  ++setVariables;
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
