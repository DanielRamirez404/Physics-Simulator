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
  functions.push_back( {"VERTICAL MOTION", &chooseVerticalMotion} );
  return functions;
}

void printNotEnoughVariablesMesssage() {
  std::cout << "NOT ENOUGH VARIABLES TO START THE SIMULATION\n";
  std::cout << "PLEASE, TRY ADDING MORE\n";
}

void countdown() {
  constexpr float seconds{3.0f};
  Timer countdown{seconds};
  countdown.setIsTimePrinted(true);
  countdown.run();
}

void chooseHorizontalMotion() {
  std::vector<MenuFunction> horizontalFunctions{};
  //horizontalFunctions.push_back( { "UNIFORM LINEAR MOTION", &doUniformLinearMotion } );
  horizontalFunctions.push_back( { "UNIFORMILY ACCELERATED LINEAR MOTION", &doUniformilyAcceleratedLinearMotion} );
  //horizontalFunctions.push_back( { "UNIFORMILY DECELERATED LINEAR MOTION", } );
  RunOnceMenu horizontalMenu("HORIZONTAL MOTION", horizontalFunctions);
  horizontalMenu.run();
}

void printValues(Motion& myMotion) { //testing function
  std::cout << "a:" << myMotion.getVariableValue('a') << '\n';
  std::cout << "V:" << myMotion.getVariableValue('V') << '\n';
  std::cout << "d:" << myMotion.getVariableValue('d') << '\n';
  std::cout << "t:" << myMotion.getVariableValue('t') << '\n';
}

void doUniformLinearMotion() {
  Motion uniformLinearMotion{};
  uniformLinearMotion.setVariable('a', 0.0f);
  while (true) {
    if (!uniformLinearMotion.isVariableSet('V')) {
      std::cout << "DO YOU HAVE THE VELOCITY VALUE? (y/n)\n";
      if (ynInput()) {
        std::cout << "ENTER THE VELOCITY VALUE\n";
        uniformLinearMotion.setVariable( 'V', getPositiveNumberInput<float>() );
      }
    }
    if (!uniformLinearMotion.isVariableSet('d')) {
      std::cout << "DO YOU HAVE THE DISTANCE VALUE? (y/n)\n";
      if (ynInput()) {
        std::cout << "ENTER THE DISTANCE VALUE\n";
        uniformLinearMotion.setVariable( 'd', getPositiveNumberInput<float>() );
      }
    }
    if (!uniformLinearMotion.isVariableSet('t')) {
      std::cout << "DO YOU HAVE THE TIME VALUE? (y/n)\n";
      if (ynInput()) {
        std::cout << "ENTER THE TIME VALUE\n";
        uniformLinearMotion.setVariable( 't', getPositiveNumberInput<float>() );
      }
    }
    if (uniformLinearMotion.areAllVariablesSet()) {
      break;
    } else {
      if (uniformLinearMotion.canDetermineRemainingVariables()) {
        uniformLinearMotion.determineRemainingVariables();
      }
    }
  }
  countdown();
  printValues(uniformLinearMotion); //testing
  pressAnyToContinue();             //testing
  uniformLinearMotion.simulate();
}

void doUniformilyAcceleratedLinearMotion() {
  Motion acceleratedMotion{};
  std::cout << "PLEASE, ENTER THE TIME VALUE (REQUIRED)\n";
  acceleratedMotion.setVariable( 't', getPositiveNumberInput<float>() );
  while (!acceleratedMotion.areAllVariablesSet()) {
    if (!acceleratedMotion.isVariableSet('a')) {
      std::cout << "DO YOU HAVE THE ACCELERATION VALUE? (y/n)\n";
      if (ynInput()) {
        std::cout << "ENTER THE ACCELERATION VALUE\n";
        acceleratedMotion.setVariable( 'a', getPositiveNumberInput<float>() );
      }
    }
    if (!acceleratedMotion.isVariableSet('o')) {
      std::cout << "DO YOU HAVE THE INITIAL VELOCITY VALUE? (y/n)\n";
      if (ynInput()) {
        std::cout << "ENTER THE INITIAL VELOCITY VALUE\n";
        acceleratedMotion.setVariable( 'o', getPositiveNumberInput<float>() );
      }
    }
    if (!acceleratedMotion.isVariableSet('V')) {
      std::cout << "DO YOU HAVE THE FINAL VELOCITY VALUE? (y/n)\n";
      if (ynInput()) {
        std::cout << "ENTER THE FINAL VELOCITY VALUE\n";
        acceleratedMotion.setVariable( 'V', getPositiveNumberInput<float>() );
      }
    }
    if (!acceleratedMotion.isVariableSet('d')) {
      std::cout << "DO YOU HAVE THE DISTANCE VALUE? (y/n)\n";
      if (ynInput()) {
        std::cout << "ENTER THE DISTANCE VALUE\n";
        acceleratedMotion.setVariable( 'd', getPositiveNumberInput<float>() );
      }
    }
    if (acceleratedMotion.canDetermineRemainingVariables()) acceleratedMotion.determineRemainingVariables();
  }
  countdown();
  printValues(acceleratedMotion);   //testing
  pressAnyToContinue();             //testing
  acceleratedMotion.simulate();
}

void chooseVerticalMotion() {
  countdown();
  Motion myMotion{};
  myMotion.simulate();
}
