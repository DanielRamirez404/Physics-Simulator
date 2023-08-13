#include "userinput.h"
#include <iostream>
#include <conio.h>
#include <string>

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
      pressAnyToContinue();
      ynInput();
      break;
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
