#include "simulator.h"
#include "motion.h"
#include "timed functions.h"
#include "userinput.h"
#include "menu.h"
#include <iostream>
#include <vector>

const std::vector<MenuFunction>& getSimulatorFunctions() {
  static std::vector<MenuFunction> functions{};
  functions.push_back( {"HORIZONTAL MOTION", &chooseHorizontalMotion} );
  //functions.push_back( {"VERTICAL MOTION", &chooseVerticalMotion} );
  return functions;
}

void chooseHorizontalMotion() {
  std::vector<MenuFunction> horizontalFunctions{};
  horizontalFunctions.push_back( { "UNIFORM LINEAR MOTION", &doUniformLinearMotion } );
  horizontalFunctions.push_back( { "UNIFORMILY VARIED LINEAR MOTION", &doUniformilyVariedLinearMotion} );
  RunOnceMenu horizontalMenu("HORIZONTAL MOTION", horizontalFunctions);
  horizontalMenu.run();
}

void printValues(Motion& myMotion) { //testing function
  std::cout << "a:" << myMotion.getVariableValue('a') << '\n';
  std::cout << "o:" << myMotion.getVariableValue('o') << '\n';
  std::cout << "V:" << myMotion.getVariableValue('V') << '\n';
  std::cout << "d:" << myMotion.getVariableValue('d') << '\n';
  std::cout << "t:" << myMotion.getVariableValue('t') << '\n';
}

void doUniformLinearMotion() {
  Motion uniformLinearMotion{};
  uniformLinearMotion.setVariable('a', 0);
  std::cout << "PLEASE, ENTER THE TIME VALUE (REQUIRED)\n";
  uniformLinearMotion.setVariable( 't', getPositiveNumberInput<float>() );
  for (auto it{ MotionVariables::list.begin() }; !uniformLinearMotion.canDetermineRemainingVariables(); it++) {
    if (it == MotionVariables::list.end()) it = MotionVariables::list.begin();
    if (uniformLinearMotion.isVariableSet(*it) || *it == 'o') continue;
    std::cout << "DO YOU HAVE THE VALUE FOR " << *it << "? (y/n)\n";
    if (ynInput()) {
      std::cout << "ENTER ITS VALUE\n";
      uniformLinearMotion.setVariable(*it, getPositiveNumberInput<float>());
      if (*it == 'V') uniformLinearMotion.setVariable('o', uniformLinearMotion.getVariableValue('V'));
    }
  }
  uniformLinearMotion.determineRemainingVariables();
  uniformLinearMotion.simulate();
}

void doUniformilyVariedLinearMotion() {
  Motion acceleratedMotion{};
  std::cout << "PLEASE, ENTER THE TIME VALUE (REQUIRED)\n";
  acceleratedMotion.setVariable( 't', getPositiveNumberInput<float>() );
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
