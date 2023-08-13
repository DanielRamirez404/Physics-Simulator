#include "simulator.h"
#include "timed functions.h"
#include "menu.h"
#include <iostream>
#include <vector>

std::vector<menuFunction> getSimulatorFunctions() {
  std::vector<menuFunction> functions{};
  functions.push_back( {"VERTICAL MOVEMENT", &chooseVerticalMovement} );
  functions.push_back( {"HORIZONTAL MOVEMENT", &chooseHorizontalMovement} );
  return functions;
}

void chooseVerticalMovement() {

}

void chooseHorizontalMovement() {
  
}
