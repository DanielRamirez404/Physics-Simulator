#pragma once
#include <functional>

class TimedFunction {
private:
  float timerSeconds{};
  std::function<void()> function{};
  bool willTimePrint{false};
  void printTime(float remainingTime);
public:
  TimedFunction(float seconds, std::function<void()> myFunction) : timerSeconds(seconds), function(myFunction) {};
  void willTimeBePrinted(bool option) { willTimePrint = option; };
  void runWithTimer();
  void runAfterTimer();
};
