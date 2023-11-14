#include "distances.h"
#include <cmath>
#include <utility>

DistancesToPrint::DistancesToPrint(float firstDistance, float secondDistance, float thirdDistance)
: first(firstDistance)
, current(secondDistance)
, last(thirdDistance) {
  while (last < 0 || first < 0) {
    float diference { last - first };
    float totalDistance { (diference > 0) ? diference : -diference };
    first += totalDistance;
    current += totalDistance;
    last += totalDistance;
  }
  if (first > last) {
    std::swap(first, last);
  }
}
