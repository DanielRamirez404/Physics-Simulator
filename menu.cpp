#include "menu.h"
#include "userinput.h"

#include <cassert>
#include <cstddef>
#include <iostream>
#include <functional>
#include <vector>

void Menu::run() {
  while (true) {
    clearConsole();
    print();
    size_t selectedOption{ getUserInput<size_t>() };
    assert((selectedOption > 0) && (selectedOption <= totalOptions + 1) && "Nonvalid option");
    if (isUserQuitting(selectedOption)) { 
      if (isQuittingConfirmed()) {
        break;
      } else {
        continue;
      }
    }
    --selectedOption;
    functions[selectedOption].function();
    pressAnyToContinue();
  }
}

bool Menu::isQuittingConfirmed() {
  std::cout << "ARE YOU SURE YOU WANT TO " << exitMessage << "? (y/n)\n";
  return ynInput();
}

void Menu::print() {
  printTitle();
  for (size_t i{0}; i < totalOptions; ++i) {
    std::cout << i + 1 << ") " << functions[i].name << '\n';
  }
  std::cout << totalOptions + 1 << ") " << exitMessage << '\n';
}

void Menu::printTitle() {
  constexpr int menuWidth{46};
  constexpr int halfWidth{menuWidth / 2};
  const int titleSize { static_cast<int>(title.size()) };
  const int halfTitleSize{titleSize / 2};
  assert((titleSize <= menuWidth) && "THE TITLE CAN\'T BE TOO LONG");
  std::cout << "----------------------------------------------\n";
  for (int i{0}; i < halfWidth - halfTitleSize; i++) {
    std::cout << ' ';
  }
  std::cout << title << '\n';
  std::cout << "----------------------------------------------\n";
}

void RunOnceMenu::run() {
  clearConsole();
  print();
  size_t selectedOption{ getUserInput<size_t>() };
  assert((selectedOption > 0) && (selectedOption <= totalOptions + 1) && "Nonvalid option");
  if (isUserQuitting(selectedOption)) {
    if (isQuittingConfirmed()) {
      return;
    } else {
      run();
    }
  --selectedOption;
  functions[selectedOption].function();
  }
}
