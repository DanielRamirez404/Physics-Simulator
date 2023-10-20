#include "motion.h"
#include "variable.h"
#include "timed functions.h"
#include "userinput.h"
#include "usermath.h"
#include <functional>
#include <algorithm>
#include <cassert>
#include <iostream>

Variable& Motion::getVariable(char identifier) {
  return *std::find_if(motionVariables.begin(), motionVariables.end(), [&](Variable& variable) { return variable.getIdentifier() == identifier; });
}

void Motion::determineRemainingVariables() {
  assert(canDetermineRemainingVariables() && "CAN\'T DETERMINE REMAINING VALUES");
  if (!getVariable('a').isSet()) {
    determineAcceleration();
    if (areAllVariablesSet()) return;
  }
  if (!getVariable('V').isSet()) {
    determineVelocity();
    if (areAllVariablesSet()) return;
  }
  if (!getVariable('d').isSet()) {
    determineDistance();
    if (areAllVariablesSet()) return;
  }
  if (!getVariable('t').isSet()) {
    determineTime();
    if (areAllVariablesSet()) return;
  }
}

void Motion::determineAcceleration() {
  if (setVariables == 3) {
    getVariable('a').set((-2 * (getVariable('d').get() - (getVariable('V').get() * getVariable('t').get()))) / Math::exponentiation(getVariable('t').get(), 2)); 
  } else if (!getVariable('V').isSet()) {
    getVariable('a').set((2 * getVariable('d').get()) / Math::exponentiation(getVariable('t').get(), 2));
  } else if (!getVariable('d').isSet()) {
    getVariable('a').set(getVariable('V').get() / getVariable('t').get());
  } else {
    getVariable('a').set(pow(getVariable('V').get(), 2) / (2 * getVariable('d').get()));
  }
  ++setVariables;
}

void Motion::determineVelocity() {
  if (setVariables == 3) {
    getVariable('V').set((getVariable('d').get() + ((getVariable('a').get() * Math::exponentiation(getVariable('t').get(), 2)) / 2)) / getVariable('t').get());
  } else if (!getVariable('a').isSet()) {
    getVariable('V').set((2 * getVariable('d').get()) / getVariable('t').get());
  } else if (!getVariable('d').isSet()) {
    getVariable('V').set(getVariable('a').get() * getVariable('t').get());
  } else {
    getVariable('V').set(sqrt(2 * getVariable('a').get() * getVariable('d').get()));
  }
  ++setVariables;
}

void Motion::determineDistance() {
  if (setVariables == 3) {
    getVariable('d').set((getVariable('V').get() * getVariable('t').get()) - ((getVariable('a').get() * Math::exponentiation(getVariable('t').get(), 2)) / 2));
  } else if (!getVariable('a').isSet()) {
    getVariable('d').set((getVariable('V').get() * getVariable('t').get()) / 2);
  } else if (!getVariable('V').isSet()) {
    getVariable('d').set((getVariable('a').get() * Math::exponentiation(getVariable('t').get(), 2)) / 2);
  } else {
    getVariable('d').set(pow(getVariable('V').get(), 2) /  (2 * getVariable('a').get()));
  }
  ++setVariables;
}

void Motion::determineTime() {
  if (setVariables == 3) {
    getVariable('t').set(sqrt(-getVariable('a').get() * ((getVariable('d').get() - (getVariable('V').get() * getVariable('t').get())) / 2)));
  } else if (!getVariable('a').isSet()) {
    getVariable('t').set((2 * getVariable('d').get()) / getVariable('V').get());
  } else if (!getVariable('V').isSet()) {
    getVariable('t').set(sqrt((2 * getVariable('d').get()) / getVariable('a').get()));
  } else {
    getVariable('t').set(getVariable('V').get() / getVariable('a').get());
  }
  ++setVariables;
}

void Motion::printCurrentState(float currentTime) {
  float currentDistance { getCurrentDistance(currentTime) };
  int relativeDistance { Math::percentage(currentDistance, getVariable('d').get()) };
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
  return ((getVariable('d').get() * currentTime) / 2);   //this needs rework
}

void Motion::simulate() {
  std::function<void(float)> printFunction { std::bind(&Motion::printCurrentState, this, std::placeholders::_1) };
  TimeUsableFunction simulation{getVariable('t').get(), printFunction};
  simulation.setIsTimePrinted(true);
  simulation.run();
}

void Motion::setVariable(char identifier, float value) {
  getVariable(identifier).set(value);
}

bool Motion::isVariableSet(char identifier) {
  return getVariable(identifier).isSet();
}

float Motion::getVariableValue(char identifier) {
  return getVariable(identifier).get();
}
