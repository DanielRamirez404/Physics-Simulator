#include "simulator.h"
#include "menu.h"
#include <vector>
#include <iostream>
#include <iomanip>

int main() {
  
  MainMenu simulator{"PHYSICS SIMULATOR", getSimulatorFunctions()};
  simulator.run();
  //exitMessage();
  return 0;
}
