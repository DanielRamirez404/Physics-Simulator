#include "simulator.h"
#include "timed functions.h"
#include "menu.h"
#include <iostream>
#include <vector>

std::vector<MenuFunction> getSimulatorFunctions() {
  std::vector<MenuFunction> functions{};
  functions.push_back( {"VERTICAL MOVEMENT", &chooseVerticalMovement} );
  functions.push_back( {"HORIZONTAL MOVEMENT", &chooseHorizontalMovement} );
  return functions;
}

void chooseVerticalMovement() {

}

void chooseHorizontalMovement() {
  
}
