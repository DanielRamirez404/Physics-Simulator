#include "distances.h"
#include <cmath>
#include <utility>

DistancesToPrint::DistancesToPrint(float firstDistance, float secondDistance, float thirdDistance)
  : first{firstDistance}, current{secondDistance}, last{thirdDistance} {
  parseLimits();
}

DistancesToPrint::DistancesToPrint(float firstDistance, float lastDistance) 
  : DistancesToPrint{firstDistance, firstDistance, lastDistance} {

}

void DistancesToPrint::parseLimits() {
  calculateTotalDistance();
  if (first > last) 
    std::swap(first, last);
  if (first < 0) {
    first += total;
    current += total;
    last += total;
  }
}

void DistancesToPrint::calculateTotalDistance() {
  total = abs(last - first);
}

void DistancesToPrint::setLimits(float firstDistance, float lastDistance) {
  first = firstDistance;
  last = lastDistance;
  parseLimits();
}

void DistancesToPrint::setCurrent(float distance) {
  current = (distance < first) ? distance + total : distance;
}
