#pragma once
#include <cstddef>
#include <string>
#include <string_view>

class String {
protected:
  std::string string{};
public:
  String() {};
  String(std::string_view myString) : string(myString) {};
  auto operator[](size_t index) { return string[index]; };
  auto at(size_t index) { return string[index]; };
  auto begin() { return string.begin(); };
  auto end() { return string.end(); };
  void add(std::string_view myString, size_t index);
  void add(char charToAdd, size_t index);
  void prepend(std::string_view myString);
  void prepend(char myChar);
  void append(std::string_view myString);
  void append(char myChar);
  void erase(size_t index);
  void erase(size_t index, size_t numberOfPositions);
  void eraseWhitespaces();
  bool contains(char myChar);
  int count(char myChar);
  size_t find(char myChar);
  size_t size();
  std::string substr(size_t startingIndex, size_t subStringSize);
  char cut(size_t index);
  std::string cut(size_t startingIndex, size_t subStringSize);
  std::string_view get();
};
