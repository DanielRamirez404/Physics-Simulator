#include "timed functions.h"
#include "functional"
#include "ctime"

void TimedFunction::run() {
  float totalTime{secondsToRun * CLOCKS_PER_SEC};
  clock_t startingTime { clock() };
  while ((clock() - startingTime) < totalTime)
    function();
}
