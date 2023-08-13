#include "simulator.h"
#include "menu.h"

int main() {
  mainMenu simulator{"PHYSICS SIMULATOR", getSimulatorFunctions()};
  simulator.run();
  //exitMessage();
  return 0;
}
