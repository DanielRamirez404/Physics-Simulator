#pragma once
#include "menu.h"
#include "motion.h"
#include <vector>

const std::vector<MenuFunction>& getSimulatorFunctions();
void chooseHorizontalMotion();
void doUniformLinearMotion();
void doUniformilyVariedLinearMotion();
void chooseVerticalMotion();
void setTimeFromUser(Motion& motion);
bool doesUserHaveValueFor(char identifier);
void setVariableFromUser(Motion& motion, char identifier);
