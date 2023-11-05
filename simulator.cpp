#include "simulator.h"
#include "motion.h"
#include "timed functions.h"
#include "userinput.h"
#include "menu.h"
#include <iostream>
#include <cstddef>
#include <vector>

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
  std::cout << "DO YOU HAVE THE VALUE FOR " << identifier << "? (y/n)\n";
  return ynInput();
}

void setVariableFromUser(Motion& motion, char identifier) {
  std::cout << "ENTER THE VALUE FOR " << identifier << '\n';
  motion.setVariable(identifier, getPositiveNumberInput<float>());
}
