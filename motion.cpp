#include "motion.h"
#include "timed functions.h"
#include "userinput.h"
#include "math.h"
#include <cmath>
#include <functional>
#include <cassert>
#include <iostream>

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

void Motion::printCurrentState(float currentTime) {
  float currentDistance { getCurrentDistance(currentTime) };
  int relativeDistance { percentage(currentDistance, distance.value) };
  constexpr int maxPrintableWidth { 100 };
  for (int i{0}; i < maxPrintableWidth - 1; ++i) {
    if (relativeDistance == i) {
      std::cout << '*';
      break;
    }
    std::cout << ' ';
  }
  clearConsole();
}

float Motion::getCurrentDistance(float currentTime) {
  return ((velocity.value * currentTime) / 2);   //this needs rework
}

void Motion::simulate() {
  std::function<void(float)> printFunction { std::bind(&Motion::printCurrentState, this, std::placeholders::_1) };
  TimeUsableFunction simulation{time.value, printFunction};
  simulation.setIsTimePrinted(true);
  simulation.run();
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
