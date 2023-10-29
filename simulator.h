#pragma once
#include "menu.h"
#include <vector>

const std::vector<MenuFunction>& getSimulatorFunctions();
void chooseHorizontalMotion();
void doUniformLinearMotion();
void doUniformilyVariedLinearMotion();
void chooseVerticalMotion();
