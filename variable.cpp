#include "variable.h"
#include <cmath>

void Variable::set(float myValue) {
  value = (isSigned) ? myValue : abs(myValue);
  hasBeenSet = true;
}

void Variable::setSignedness(bool myBool) {
  isSigned = myBool;
  if (hasBeenSet && !isSigned) 
    value = abs(value);
}

float Variable::get() const {
  return value;
}

bool Variable::isSet() const {
  return hasBeenSet;
}

char Variable::getIdentifier() const {
  return identifier;
}
