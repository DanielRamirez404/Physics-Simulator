#include "motion.h"
#include "variable.h"
#include "timed functions.h"
#include "userinput.h"
#include "equation.h"
#include "distances.h"
#include "usermath.h"
#include "math characters.h"
#include "uservector.h"
#include <functional>
#include <string>
#include <array>
#include <string_view>
#include <algorithm>
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

bool Motion::canSimulate() {
  return areAllVariablesSet() && getVariableValue(MotionIdentifiers::distance) != 0.0f;
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
  distances.setCurrent(getCurrentDistance(currentTime));
  const unsigned int relativeDistance { distances.getRelativeDistance() };
  //art found in https://ascii.co.uk/art/car
  clearConsole();
  std::cout << "----------------------------------------------\n";
  std::cout << "ELAPSED TIME: " << Math::round(currentTime) << " SECONDS\n";
  std::cout << "CURRENT DISTANCE: " << Math::round(distances.getCurrent()) << " METERS\n";
  std::cout << "----------------------------------------------\n";
  printlInDistance("                     .------.                    "   , relativeDistance);
  printlInDistance("                    :|||\"\"\"`.`.                  ", relativeDistance);
  printlInDistance("                    :|||     7.`.                "   , relativeDistance);
  printlInDistance("  .===+===+===+===+===||`----L7'-`7`---.._       "   , relativeDistance);
  printlInDistance("  []                  || ==       |       \"\"\"-.  ", relativeDistance);
  printlInDistance("  []...._____.........||........../ _____ ____|  "   , relativeDistance);
  printlInDistance(" c\\____/,---.\\_       ||_________/ /,---.\\_  _/  ", relativeDistance);
  printlInDistance("   /_,-/ ,-. \\ `._____|__________||/ ,-. \\ \\_[   ", relativeDistance);
  printlInDistance("      /\\ `-' /                    /\\ `-' /       " , relativeDistance);
  printlInDistance("        `---'                       `---'        "   , relativeDistance);
}

void Motion::printlInDistance(std::string_view string, unsigned int distance) {
  using Distance = unsigned int;
  constexpr Distance maxPrintableWidth { 100 };
  if (string.size() > distance) {
    string.remove_prefix(string.size() - distance);
    std::cout << string << '\n';
    return;
  }
  const Distance distanceToPrint(distance - string.size());
  if (distanceToPrint > maxPrintableWidth) string.remove_suffix(distanceToPrint - maxPrintableWidth);
  for (Distance i{0}; i <= maxPrintableWidth; ++i) {
    if (i == distanceToPrint) {
      std::cout << string << '\n';
      break;
    }
    std::cout << ' ';
  }
}

float Motion::getCurrentDistance(float currentTime) { // d = o * t + (a * t ^ 2) / 2
  return getVariable(MotionIdentifiers::initialVelocity).get() * currentTime + (getVariable(MotionIdentifiers::acceleration).get() * Math::exponentiation<float>(currentTime, 2)) / 2;
}

void Motion::simulate() {
  distances.setLimits(getCurrentDistance(0), getVariable(MotionIdentifiers::distance).get());
  std::function<void(float)> printFunction { std::bind(&Motion::printCurrentState, this, std::placeholders::_1) };
  TimeUsableFunction simulation{getVariable(MotionIdentifiers::time).get(), printFunction};
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
