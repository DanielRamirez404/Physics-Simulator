#pragma once
#include <functional>

void doNothing();

class TimedFunction {
protected:
  float timerSeconds{};
  std::function<void()> function{};
  bool willTimePrint{false};
  void printTime(float remainingTime);
public:
  TimedFunction(float seconds, std::function<void()> myFunction) : timerSeconds(seconds), function(myFunction) {};
  void willTimeBePrinted(bool option) { willTimePrint = option; };
  void run();
};

class DelayedFunction : TimedFunction {
public:
  using TimedFunction::TimedFunction;
  void run();
};

class TimeUsableFunction : TimedFunction {
private:
  std::function<void(float)> timeUsableFunction{};
public:
  TimeUsableFunction(float seconds, std::function<void(float)> myfunction) : TimedFunction(seconds, &doNothing) { timeUsableFunction = myfunction; };
  void run();
};
