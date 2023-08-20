#pragma once
#include "menu.h"
#include <vector>

std::vector<MenuFunction> getSimulatorFunctions();
void printNotEnoughVariablesMesssage();
void countdown();
void chooseHorizontalMotion();
void doUniformLinearMotion();
void chooseVerticalMotion();
