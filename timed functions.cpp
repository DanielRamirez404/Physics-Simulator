#include "timed functions.h"
#include <functional>
#include <ctime>
#include <iostream>

void TimedFunction::printTime(float remainingTime) {
  std::cout << "----------------------------------------------\n";
  std::cout << "TIME LEFT: " << remainingTime << " SECONDS\n";
  std::cout << "----------------------------------------------\n";
}

void TimedFunction::runWithTimer() {
  auto totalTime {timerSeconds * CLOCKS_PER_SEC};
  auto startingTime { clock() };
  auto elapsedTime {clock() - startingTime};
  while (elapsedTime < totalTime) {
    if (willTimePrint) printTime((totalTime - elapsedTime) / CLOCKS_PER_SEC);
    function();
    elapsedTime = clock() - startingTime;
  }
}

void TimedFunction::runAfterTimer() {
  auto totalTime{timerSeconds * CLOCKS_PER_SEC};
  auto startingTime { clock() };
  while ((clock() - startingTime) < totalTime);
  function();
}
