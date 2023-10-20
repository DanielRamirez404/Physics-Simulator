#include "variable.h"

void Variable::set(float myValue) {
  value = myValue;
  hasBeenSet = true;
}

float Variable::get() {
  return value;
}

bool Variable::isSet() {
  return hasBeenSet;
}

char Variable::getIdentifier() {
  return identifier;
}
