#include "simulator.h"
#include "menu.h"

int main() {
  MainMenu simulator{"PHYSICS SIMULATOR", getSimulatorFunctions()};
  simulator.run();
  //exitMessage();
  return 0;
}
