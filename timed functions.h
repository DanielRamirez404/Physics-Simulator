#pragma once
#include <functional>

class TimedFunction {
private:
  float timerSeconds{};
  std::function<void()> function{};
public:
  TimedFunction(float seconds, std::function<void()> myFunction) : timerSeconds(seconds), function(myFunction) {};
  void runWithTimer();
  void runAfterTimer();
};
