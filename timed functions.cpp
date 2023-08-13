#include "timed functions.h"
#include "functional"
#include "ctime"

void TimedFunction::runWithTimer() {
  auto totalTime{timerSeconds * CLOCKS_PER_SEC};
  auto startingTime { clock() };
  while ((clock() - startingTime) < totalTime) {
    function();
  }
}

void TimedFunction::runAfterTimer() {
  auto totalTime{timerSeconds * CLOCKS_PER_SEC};
  auto startingTime { clock() };
  while ((clock() - startingTime) < totalTime);
  function();
}
