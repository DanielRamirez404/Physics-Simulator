#pragma once
#include <cstddef>
#include <functional>
#include <string>
#include <vector>

struct menuFunction {
  std::string name;
  std::function<void()> function;
};

class menu {
protected:
  std::string exitMessage{"GO BACK"};
  std::string title{};
  size_t totalOptions{};
  std::vector<menuFunction> functions{};
  bool isUserQuitting(size_t selectedOption) { return (selectedOption == totalOptions + 1); };
  bool isQuittingConfirmed();
  void printTitle();
public:
  menu (const menu&) = delete;
  menu& operator=(const menu&) = delete; 
  menu(std::string menuTitle, std::vector<menuFunction> menuFunctions);
  void run();
  void print();
};

class mainMenu : public menu {
public:
  mainMenu(std::string menuTitle, std::vector<menuFunction> menuFunctions) : menu (menuTitle, menuFunctions) { menu::exitMessage = "EXIT"; };
};

class runOnceMenu : public menu {
private:
  bool didUserQuit{false};
public:
  using menu::menu;
  void run();
  bool didUserExit() { return didUserQuit; } ;
};
