#include "simulator.h"
#include "motion.h"
#include "timed functions.h"
#include "userinput.h"
#include "menu.h"
#include <iostream>
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
  Motion uniformLinearMotion{};
  uniformLinearMotion.setVariable(MotionVariables::identifiers::acceleration, 0);
  std::cout << "PLEASE, ENTER THE TIME VALUE (REQUIRED)\n";
  uniformLinearMotion.setVariable(MotionVariables::identifiers::time, getPositiveNumberInput<float>());
  for (auto it{ MotionVariables::list.begin() }; !uniformLinearMotion.canDetermineRemainingVariables(); it++) {
    if (it == MotionVariables::list.end()) it = MotionVariables::list.begin();
    if (uniformLinearMotion.isVariableSet(*it) || *it == MotionVariables::identifiers::initialVelocity) continue;
    std::cout << "DO YOU HAVE THE VALUE FOR " << *it << "? (y/n)\n";
    if (ynInput()) {
      std::cout << "ENTER ITS VALUE\n";
      uniformLinearMotion.setVariable(*it, getPositiveNumberInput<float>());
      if (*it == MotionVariables::identifiers::finalVelocity) uniformLinearMotion.setVariable(MotionVariables::identifiers::initialVelocity, uniformLinearMotion.getVariableValue(MotionVariables::identifiers::finalVelocity));
    }
  }
  uniformLinearMotion.determineRemainingVariables();
  uniformLinearMotion.simulate();
}

void doUniformilyVariedLinearMotion() {
  Motion acceleratedMotion{};
  std::cout << "PLEASE, ENTER THE TIME VALUE (REQUIRED)\n";
  acceleratedMotion.setVariable(MotionVariables::identifiers::time, getPositiveNumberInput<float>());
  for (auto it{ MotionVariables::list.begin() }; !acceleratedMotion.canDetermineRemainingVariables(); it++) {
    if (it == MotionVariables::list.end()) it = MotionVariables::list.begin();
    if (acceleratedMotion.isVariableSet(*it)) continue;
    std::cout << "DO YOU HAVE THE VALUE FOR " << *it << "? (y/n)\n";
    if (ynInput()) {
      std::cout << "ENTER ITS VALUE\n";
      acceleratedMotion.setVariable(*it, getPositiveNumberInput<float>());
    }
  }
  acceleratedMotion.determineRemainingVariables();
  acceleratedMotion.simulate();
}

void chooseVerticalMotion() {
  Motion myMotion{};
  myMotion.simulate();
}
