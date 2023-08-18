#pragma once
#include <iostream>
#include <string>

void ignoreExceedingInput();
void handleFailedInput();

template <typename T> T getUserInput() {
  T userInput{};
  while (true) {
    std::cout << "----------------------------------------------\n";
    std::cout << "INPUT: ";
    std::cin >> userInput;
    if (std::cin.fail())
      handleFailedInput();
    else
      break;
  }
  std::cout << "----------------------------------------------\n";
  ignoreExceedingInput();
  return userInput;
}

std::string getUserInputLine();
bool ynInput();
void printBadInputError();
void pressAnyToContinue();
void clearConsole();
