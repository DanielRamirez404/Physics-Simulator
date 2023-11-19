#pragma once

class DistancesToPrint {
private:
  float first{};
  float current{};
  float last{};
  float total{};
public:
  DistancesToPrint() = default;
  DistancesToPrint(float firstDistance, float lastDistance);
  DistancesToPrint(float firstDistance, float secondDistance, float thirdDistance);
  unsigned int getRelativeDistance() const { return static_cast<unsigned int>((current / last) * 100); };
  void parseLimits();
  void calculateTotalDistance();
  float getCurrent() const { return current; };
  void setLimits(float firstDistance, float lastDistance);
  void setCurrent(float distance);
};
