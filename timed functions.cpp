#include "timed functions.h"
#include "userinput.h"
#include <functional>
#include <ctime>
#include <iostream>

void Timer::printTime(float remainingTime) const {
  std::cout << "----------------------------------------------\n";
  std::cout << "TIME LEFT: " << remainingTime << " SECONDS\n";
  std::cout << "----------------------------------------------\n";
}

void Timer::run() const {
  const auto totalTime{seconds * CLOCKS_PER_SEC};
  const auto startingTime { clock() };
  auto elapsedTime { clock() - startingTime };
  while (elapsedTime < totalTime) {
    if (isTimePrinted) {
      auto remainingTime { (totalTime - elapsedTime) / CLOCKS_PER_SEC };
      printTime(remainingTime);
      clearConsole();
    }
    elapsedTime = clock() - startingTime;
  }
}

void TimedFunction::run() const {
  const auto totalTime {seconds * CLOCKS_PER_SEC};
  const auto startingTime { clock() };
  auto elapsedTime {clock() - startingTime};
  while (elapsedTime < totalTime) {
    if (isTimePrinted) {
      auto remainingTime { (totalTime - elapsedTime) / CLOCKS_PER_SEC };
      printTime(remainingTime);
    }
    function();
    elapsedTime = clock() - startingTime;
  }
}

void TimeUsableFunction::run() const {
  const auto totalTime {seconds * CLOCKS_PER_SEC};
  const auto startingTime { clock() };
  auto elapsedTime {clock() - startingTime};
  while (elapsedTime < totalTime) {
    if (isTimePrinted) {
      auto remainingTime { (totalTime - elapsedTime) / CLOCKS_PER_SEC };
      printTime(remainingTime);
    }
    function( static_cast<float>(elapsedTime) / CLOCKS_PER_SEC );
    Timer waitingTime{1.0f / TimeUsableFunction::fps};
    waitingTime.run();
    elapsedTime = clock() - startingTime;
  }
}
