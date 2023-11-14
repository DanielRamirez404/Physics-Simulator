#pragma once

class DistancesToPrint {
private:
  float first{};
  float current{};
  float last{};
public:
  DistancesToPrint(float firstDistance, float secondDistance, float thirdDistance);
  unsigned int getRelativeDistance() const { return static_cast<unsigned int>((current / last) * 100); };
  float getCurrent() const { return current; };
  void setCurrent(float distance) { current = distance; };
};
