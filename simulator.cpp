#include "simulator.h"
#include "motion.h"
#include "timed functions.h"
#include "userinput.h"
#include "menu.h"
#include <iostream>
#include <vector>

std::vector<MenuFunction> getSimulatorFunctions() {
  std::vector<MenuFunction> functions{};
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
  TimedFunction countdown{seconds, &clearConsole};
  countdown.willTimeBePrinted(true);
  countdown.run();
}

void chooseHorizontalMotion() {
  std::vector<MenuFunction> horizontalFunctions{};
  horizontalFunctions.push_back( { "UNIFORM LINEAR MOTION", &doUniformLinearMotion } );
  //horizontalFunctions.push_back( { "UNIFORMILY ACCELERATED LINEAR MOTION", } );
  //horizontalFunctions.push_back( { "UNIFORMILY DECELERATED LINEAR MOTION", } );
  RunOnceMenu horizontalMenu("HORIZONTAL MOTION", horizontalFunctions);
  horizontalMenu.run();
}

void printValues(Motion& myMotion) { //testing function
  std::cout << "a:" << myMotion.getAcceleration() << '\n';
  std::cout << "V:" << myMotion.getVelocity() << '\n';
  std::cout << "x:" << myMotion.getDistance() << '\n';
  std::cout << "t:" << myMotion.getTime() << '\n';
}

void doUniformLinearMotion() {
  Motion uniformLinearMotion{};
  uniformLinearMotion.setAcceleration(0.0f);
  while (true) {
    if (!uniformLinearMotion.hasVelocityBeenSet()) {
      std::cout << "DO YOU HAVE THE VELOCITY VALUE? (y/n)\n";
      if (ynInput()) {
        std::cout << "ENTER THE VELOCITY VALUE\n";
        uniformLinearMotion.setVelocity( getPositiveNumberInput<float>() );
      }
    }
    if (!uniformLinearMotion.hasDistanceBeenSet()) {
      std::cout << "DO YOU HAVE THE DISTANCE VALUE? (y/n)\n";
      if (ynInput()) {
        std::cout << "ENTER THE DISTANCE VALUE\n";
        uniformLinearMotion.setDistance( getPositiveNumberInput<float>() );
      }
    }
    if (!uniformLinearMotion.hasTimeBeenSet()) {
      std::cout << "DO YOU HAVE THE TIME VALUE? (y/n)\n";
      if (ynInput()) {
        std::cout << "ENTER THE TIME VALUE\n";
        uniformLinearMotion.setTime( getPositiveNumberInput<float>() );
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
  printValues(uniformLinearMotion);
  //uniformLinearMotion.simulate();
}

void chooseVerticalMotion() {
  countdown();
}
