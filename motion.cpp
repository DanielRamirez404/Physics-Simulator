#include "motion.h"
#include "variable.h"
#include "timed functions.h"
#include "userinput.h"
#include "equation.h"
#include "usermath.h"
#include "math characters.h"
#include "uservector.h"
#include <functional>
#include <string>
#include <array>
#include <string_view>
#include <algorithm>
#include <cassert>
#include <iostream>

Variable& Motion::getVariable(char identifier) {
  return *std::find_if(variables.begin(), variables.end(), [&](Variable& variable) { return variable.getIdentifier() == identifier; });
}

std::string_view Motion::getFormulaFor(char identifier) {
  constexpr std::array<std::string_view, 5> motionFormulas { 
    "V = o + a * t",
    "d = o * t + (a * t ^ 2) / 2",
    "V ^ 2 = o ^ 2 + (2 * d * a)",
    "d = (o + V) * t / 2",
    "d = V * t - (a * t ^ 2) / 2"
  };
  return *std::find_if(motionFormulas.begin(), motionFormulas.end(), [&](std::string_view formula) {
    return formula.find(identifier) != std::string::npos && countUnknownVariables(formula) == 1;
  });
}

std::vector<char> Motion::getVariablesFor(char identifier) {
  std::string_view formula { getFormulaFor(identifier) };
  std::vector<char> formulaVariables{};
  std::for_each(formula.begin(), formula.end(), [&](char myChar) {
    if (!(myChar == ' ' || myChar == '=' || Math::isMathRelated(myChar)) && !Vector::doesElementExist<char>(formulaVariables, myChar))
      formulaVariables.push_back(myChar);
  });
  return formulaVariables;
}

int Motion::countUnknownVariables(std::string_view formula) {
  return std::count_if(variables.begin(), variables.end(), [&](Variable& variable) {
    return !(formula.find(variable.getIdentifier()) == std::string::npos || variable.isSet());
  });
}

int Motion::countSetVariables() {
  return std::count_if(variables.begin(), variables.end(), [&](Variable& variable) { return variable.isSet(); });
}

bool Motion::areAllVariablesSet() {
  return countSetVariables() == static_cast<int>(variables.size());
}

bool Motion::canDetermineRemainingVariables() {
  return (countSetVariables() >= static_cast<int>(variables.size()) - 2) && !areAllVariablesSet();
}

void Motion::determineVariable(char identifier) {
  Variable& unknownVariable { getVariable(identifier) };
  std::vector<char> formulaVariables { getVariablesFor(identifier) };
  Math::Equation<float> equation { getFormulaFor(identifier), formulaVariables};
  std::for_each(formulaVariables.begin(), formulaVariables.end(), [&](char myChar) {
    if (myChar != identifier)
      equation.addValueFor(myChar, getVariable(myChar).get());
  });
  unknownVariable.set( equation.solveFor(identifier) );
}

void Motion::determineRemainingVariables() {
  std::for_each(variables.begin(), variables.end(), [&](Variable& variable) { 
    if (!variable.isSet()) 
      determineVariable(variable.getIdentifier());
  });
}

void Motion::printCurrentState(float currentTime) {
  float currentDistance { getCurrentDistance(currentTime) };
  int relativeDistance { Math::percentage(currentDistance, getVariable('d').get()) };
  constexpr int maxPrintableWidth { 100 };
  for (int i{0}; i <= maxPrintableWidth; ++i) {
    if (relativeDistance == i) {
      std::cout << '*';
      break;
    }
    std::cout << ' ';
  }
  clearConsole();
}

float Motion::getCurrentDistance(float currentTime) { // d = o * t + (a * t ^ 2) / 2
  return getVariable('o').get() * currentTime + (getVariable('a').get() * Math::exponentiation<float>(currentTime, 2)) / 2;
}

void Motion::simulate() {
  std::function<void(float)> printFunction { std::bind(&Motion::printCurrentState, this, std::placeholders::_1) };
  TimeUsableFunction simulation{getVariable('t').get(), printFunction};
  simulation.setIsTimePrinted(true);
  simulation.run();
}

void Motion::setVariable(char identifier, float value) {
  getVariable(identifier).set(value);
}

bool Motion::isVariableSet(char identifier) {
  return getVariable(identifier).isSet();
}

float Motion::getVariableValue(char identifier) {
  return getVariable(identifier).get();
}
