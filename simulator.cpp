#include "simulator.h"
#include "motion.h"
#include "timed functions.h"
#include "userinput.h"
#include "menu.h"
#include <iostream>
#include <cstddef>
#include <vector>
#include <cmath>

const std::vector<MenuFunction>& getSimulatorFunctions() {
  static const std::vector<MenuFunction> functions{
    {"HORIZONTAL MOTION", &chooseHorizontalMotion},
    {"VERTICAL MOTION", &chooseVerticalMotion},
  };
  return functions;
}

void chooseHorizontalMotion() {
  const std::vector<MenuFunction> horizontalFunctions{
    { "UNIFORM LINEAR MOTION", &doUniformLinearMotion },
    { "UNIFORMILY VARIED LINEAR MOTION", &doUniformilyVariedLinearMotion},
  };
  RunOnceMenu horizontalMenu("HORIZONTAL MOTION", horizontalFunctions);
  horizontalMenu.run();
}

void doUniformLinearMotion() {
  using namespace MotionIdentifiers;
  Motion uniformLinearMotion{};
  setTimeFromUser(uniformLinearMotion);
  uniformLinearMotion.setVariable(acceleration, 0);
  for (size_t i{ 0 }; !uniformLinearMotion.canDetermineRemainingVariables(); i++) {
    const char identifier { identifiersList[i % identifiersList.size()] };
    if (uniformLinearMotion.isVariableSet(identifier) || identifier == initialVelocity)
      continue;
    if (doesUserHaveValueFor(identifier)) {
      setVariableFromUser(uniformLinearMotion, identifier);
      if (identifier == finalVelocity)
        uniformLinearMotion.setVariable(initialVelocity, uniformLinearMotion.getVariableValue(finalVelocity));
    }
  }
  uniformLinearMotion.determineRemainingVariables();
  uniformLinearMotion.simulate();
}

void doUniformilyVariedLinearMotion() {
  using namespace MotionIdentifiers;
  Motion acceleratedMotion{};
  setTimeFromUser(acceleratedMotion);
  for (size_t i{ 0 }; !acceleratedMotion.canDetermineRemainingVariables(); i++) {
    const char identifier { identifiersList[i % identifiersList.size()] };
    if (acceleratedMotion.isVariableSet(identifier)) 
      continue;
    if (doesUserHaveValueFor(identifier)) 
      setVariableFromUser(acceleratedMotion, identifier);
  }
  acceleratedMotion.determineRemainingVariables();
  acceleratedMotion.simulate();
}

void chooseVerticalMotion() {
  Motion myMotion{};
  myMotion.simulate();
}

void setTimeFromUser(Motion& motion) {
  std::cout << "PLEASE, ENTER THE TIME VALUE (REQUIRED)\n";
  motion.setVariable(MotionIdentifiers::time, getPositiveNumberInput<float>());
}

bool doesUserHaveValueFor(char identifier) {
  std::cout << "DO YOU HAVE THE VALUE OF THE " << getIdentifierName(identifier) << "? (y/n)\n";
  return ynInput();
}

void setVariableFromUser(Motion& motion, char identifier) {
  std::cout << "ENTER THE VALUE OF THE " << getIdentifierName(identifier) << '\n';
  (identifier == MotionIdentifiers::distance || identifier == MotionIdentifiers::time)
    ? motion.setVariable(identifier, getPositiveNumberInput<float>())
    : motion.setVariable(identifier, getUserInput<float>());
}

std::string_view getIdentifierName(char identifier) {
  switch (identifier) {
    case MotionIdentifiers::acceleration:
      return "ACCELERATION";
    case MotionIdentifiers::initialVelocity:
      return "INITIAL VELOCITY";
    case MotionIdentifiers::finalVelocity:
      return "FINAL VELOCITY";
    case MotionIdentifiers::distance:
      return "DISTANCE";
    case MotionIdentifiers::time:
      return "TIME";
  }
  return "";
}
