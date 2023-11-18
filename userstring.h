#pragma once
#include <cstddef>
#include <string>
#include <string_view>

class String {
protected:
  std::string string{};
public:
  String() = default;
  explicit String(std::string_view myString) : string(myString) {};
  auto operator[](size_t index) const { return string[index]; };
  auto at(size_t index) const { return string[index]; };
  auto begin() const { return string.begin(); };
  auto end() const { return string.end(); };
  void add(std::string_view myString, size_t index);
  void add(char charToAdd, size_t index);
  void prepend(std::string_view myString);
  void prepend(char myChar);
  void append(std::string_view myString);
  void append(char myChar);
  void erase(size_t index);
  void erase(size_t index, size_t numberOfPositions);
  void eraseWhitespaces();
  bool contains(char myChar) const;
  int count(char myChar) const;
  char cut(size_t index);
  std::string cut(size_t startingIndex, size_t subStringSize);
  std::string substr(size_t startingIndex, size_t subStringSize);
  std::string_view get() const;
  size_t find(char myChar) const;
  size_t size() const;
};
