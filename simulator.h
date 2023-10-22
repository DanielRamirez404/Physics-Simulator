#pragma once
#include "menu.h"
#include <vector>

const std::vector<MenuFunction>& getSimulatorFunctions();
void printNotEnoughVariablesMesssage();
void countdown();
void chooseHorizontalMotion();
void doUniformLinearMotion();
void doUniformilyAcceleratedLinearMotion();
void doUniformilyDeceleratedLinearMotion();
void chooseVerticalMotion();
