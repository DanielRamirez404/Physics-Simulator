#pragma once
#include <functional>

class Timer {
protected:
  float seconds{};
  bool isTimePrinted{false};
  void printTime(float remainingTime) const;
public:
  explicit Timer(float mySeconds) : seconds{mySeconds} {};
  void run() const;
  void setIsTimePrinted(bool option) { isTimePrinted = option; };
};

class TimedFunction : public Timer {
protected:
  std::function<void()> function{};
public:
  TimedFunction(float mySeconds, std::function<void()> myFunction) : Timer{mySeconds}, function{myFunction} {};
  void run() const;
};

class TimeUsableFunction : public Timer {
private:
  const int fps{60};
  std::function<void(float)> function{};
public:
  TimeUsableFunction(float mySeconds, std::function<void(float)> myFunction) : Timer{mySeconds}, function{myFunction} {};
  void run() const;
};
