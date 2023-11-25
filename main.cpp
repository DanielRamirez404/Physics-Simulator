#include "simulator.h"
#include "menu.h"

int main() {
  chooseHorizontalMotion(); // The whole program will run until the user decides to leave
  showExitMessage();        // All movements implemented are horizontal
  return 0;                 // I will eventually update this and implement more
}                           // But this is enough practice for the time being :)
