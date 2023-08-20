#pragma once
#include <iostream>
#include <string>

void ignoreExceedingInput();
void handleFailedInput();
void printBadInputError();

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

template <typename T> T getPositiveNumberInput() {
  T inputNumber{};
  while (true) {
    inputNumber = getUserInput<T>();
    if (inputNumber > 0) {
      break;
    }
    printBadInputError();
  }
  return inputNumber;
}

std::string getUserInputLine();
bool ynInput();
void pressAnyToContinue();
void clearConsole();
