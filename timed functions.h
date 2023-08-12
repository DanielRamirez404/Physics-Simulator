#pragma once
#include <functional>

class TimedFunction 
{
private:
  float secondsToRun{};
  std::function<void()> function{};
public:
  TimedFunction(float seconds, std::function<void()> myFunction) : secondsToRun(seconds), function(myFunction) {};
  void run();
};
