#include "timed functions.h"
#include <functional>
#include <ctime>
#include <iostream>

void doNothing() { }

void TimedFunction::printTime(float remainingTime) {
  std::cout << "----------------------------------------------\n";
  std::cout << "TIME LEFT: " << remainingTime << " SECONDS\n";
  std::cout << "----------------------------------------------\n";
}

void TimedFunction::run() {
  auto totalTime {timerSeconds * CLOCKS_PER_SEC};
  auto startingTime { clock() };
  auto elapsedTime {clock() - startingTime};
  while (elapsedTime < totalTime) {
    if (willTimePrint) printTime((totalTime - elapsedTime) / CLOCKS_PER_SEC);
    function();
    elapsedTime = clock() - startingTime;
  }
}

void DelayedFunction::run() {
  auto totalTime{timerSeconds * CLOCKS_PER_SEC};
  auto startingTime { clock() };
  while ((clock() - startingTime) < totalTime);
  function();
}

void TimeUsableFunction::run() {
  auto totalTime {timerSeconds * CLOCKS_PER_SEC};
  auto startingTime { clock() };
  auto elapsedTime {clock() - startingTime};
  while (elapsedTime < totalTime) {
    if (willTimePrint) printTime((totalTime - elapsedTime) / CLOCKS_PER_SEC);
    timeUsableFunction(elapsedTime);
    elapsedTime = clock() - startingTime;
  }
}
