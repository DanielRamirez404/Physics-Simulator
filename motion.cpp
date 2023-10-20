#include "motion.h"
#include "timed functions.h"
#include "userinput.h"
#include "usermath.h"
#include <functional>
#include <cassert>
#include <iostream>

void Motion::determineRemainingVariables() {
  assert(canDetermineRemainingVariables() && "CAN\'T DETERMINE REMAINING VALUES");
  if (!acceleration.isSet()) {
    determineAcceleration();
    if (areAllVariablesSet()) return;
  }
  if (!velocity.isSet()) {
    determineVelocity();
    if (areAllVariablesSet()) return;
  }
  if (!distance.isSet()) {
    determineDistance();
    if (areAllVariablesSet()) return;
  }
  if (!time.isSet()) {
    determineTime();
    if (areAllVariablesSet()) return;
  }
}

void Motion::determineAcceleration() {
  if (setVariables == 3) {
    acceleration.set((-2 * (distance.get() - (velocity.get() * time.get()))) / Math::exponentiation(time.get(), 2)); 
  } else if (!velocity.isSet()) {
    acceleration.set((2 * distance.get()) / Math::exponentiation(time.get(), 2));
  } else if (!distance.isSet()) {
    acceleration.set(velocity.get() / time.get());
  } else {
    acceleration.set(pow(velocity.get(), 2) / (2 * distance.get()));
  }
  ++setVariables;
}

void Motion::determineVelocity() {
  if (setVariables == 3) {
    velocity.set((distance.get() + ((acceleration.get() * Math::exponentiation(time.get(), 2)) / 2)) / time.get());
  } else if (!acceleration.isSet()) {
    velocity.set((2 * distance.get()) / time.get());
  } else if (!distance.isSet()) {
    velocity.set(acceleration.get() * time.get());
  } else {
    velocity.set(sqrt(2 * acceleration.get() * distance.get()));
  }
  ++setVariables;
}

void Motion::determineDistance() {
  if (setVariables == 3) {
    distance.set((velocity.get() * time.get()) - ((acceleration.get() * Math::exponentiation(time.get(), 2)) / 2));
  } else if (!acceleration.isSet()) {
    distance.set((velocity.get() * time.get()) / 2);
  } else if (!velocity.isSet()) {
    distance.set((acceleration.get() * Math::exponentiation(time.get(), 2)) / 2);
  } else {
    distance.set(pow(velocity.get(), 2) /  (2 * acceleration.get()));
  }
  ++setVariables;
}

void Motion::determineTime() {
  if (setVariables == 3) {
    time.set(sqrt(-acceleration.get() * ((distance.get() - (velocity.get() * time.get())) / 2)));
  } else if (!acceleration.isSet()) {
    time.set((2 * distance.get()) / velocity.get());
  } else if (!velocity.isSet()) {
    time.set(sqrt((2 * distance.get()) / acceleration.get()));
  } else {
    time.set(velocity.get() / acceleration.get());
  }
  ++setVariables;
}

void Motion::printCurrentState(float currentTime) {
  float currentDistance { getCurrentDistance(currentTime) };
  int relativeDistance { Math::percentage(currentDistance, distance.get()) };
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
  return ((velocity.get() * currentTime) / 2);   //this needs rework
}

void Motion::simulate() {
  std::function<void(float)> printFunction { std::bind(&Motion::printCurrentState, this, std::placeholders::_1) };
  TimeUsableFunction simulation{time.get(), printFunction};
  simulation.setIsTimePrinted(true);
  simulation.run();
}

void Motion::setAcceleration(float myAcceleration) {
  acceleration.set(myAcceleration);
  ++setVariables;
}

void Motion::setVelocity(float myVelocity) {
  velocity.set(myVelocity);
  ++setVariables;
}

void Motion::setDistance(float myDistance) {
  distance.set(myDistance);
  ++setVariables;
}

void Motion::setTime(float myTime) {
  time.set(myTime);
  ++setVariables;
}
