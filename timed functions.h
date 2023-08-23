#pragma once
#include <functional>

void doNothing();

class Timer {
protected:
  float seconds{};
  bool isTimePrinted{false};
  void printTime(float remainingTime);
public:
  Timer(float mySeconds) : seconds(mySeconds) {};
  void run();
  void setIsTimePrinted(bool option) { isTimePrinted = option; };
};

class TimedFunction : Timer {
protected:
  std::function<void()> function{};
public:
  TimedFunction(float mySeconds, std::function<void()> myFunction) : Timer(mySeconds), function(myFunction) {};
  void run();
};

class TimeUsableFunction : Timer {
private:
  std::function<void(float)> function{};
public:
  TimeUsableFunction(float mySeconds, std::function<void(float)> myFunction) : Timer(mySeconds), function(myFunction) {};
  void run();
};
