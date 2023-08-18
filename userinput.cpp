#include "userinput.h"
#include <conio.h>
#include <iostream>
#include <limits>
#include <string>

void ignoreExceedingInput() {
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void handleFailedInput() {
  if (std::cin.eof()) {
    std::cout << "Error: Input operation cannot be fixed\n";
    std::cout << "Closing the program...\n";
    exit(1);
  }
  std::cin.clear();
  ignoreExceedingInput();
  printBadInputError();
}

std::string getUserInputLine() {
  std::cout << "----------------------------------------------\n";
  std::cout << "INPUT LINE:\n";
  std::string userInput{};
  std::getline(std::cin >> std::ws, userInput);
  std::cout << "----------------------------------------------\n";
  return userInput;
}

bool ynInput() {
  bool choice{};
  while (true) {
    char userInput{ getUserInput<char>() };
    switch (tolower(userInput)) {
      case 'y':
        choice = true;
        break;
      case 'n':
        choice = false;
        break;
      default:
        printBadInputError();
        break;
    }
  }
  return choice;
}

void printBadInputError() {
  std::cout << "----------------------------------------------\n";
  std::cout << "OH-OH, IT SEEMS THAT YOUR INPUT ISN'T VALID.\n";
  std::cout << "PLEASE, TRY AGAIN.\n";
}

void pressAnyToContinue() {
  std::cout << "----------------------------------------------\n";
  std::cout << "       NOW, PRESS ANY KEY TO CONTINUE\n";
  std::cout << "----------------------------------------------\n";
  getch();
}

void clearConsole() {
  std::cout << std::string( 100, '\n' );
}
