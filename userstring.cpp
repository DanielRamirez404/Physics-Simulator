#include "userstring.h"
#include <cstddef>
#include <string>
#include <string_view>
#include <algorithm>

void String::add(std::string_view stringToAdd, size_t index) {
  (index >= string.size()) ? string.append(stringToAdd) : string.insert(index, stringToAdd);
}

void String::add(char charToAdd, size_t index) {
  if (index >= string.size())
    string += charToAdd;
  else
    string.insert(index, 1, charToAdd);
}

void String::prepend(char myChar) {
  string.insert(0, 1, myChar);
}

void String::append(char myChar) {
  string += myChar;
}

void String::erase(size_t index, size_t numberOfPositions) {
  string.erase(index, numberOfPositions);
}

void String::eraseWhitespaces() {
  for (size_t i{0}; i < string.size(); ++i) {
    while (string[i] == ' ') {
      string.erase(string.begin() + static_cast<int>(i));
    }
  }
}

bool String::contains(char myChar) {
  return string.find(myChar) != std::string::npos;
}

int String::count(char myChar) {
  return std::count(string.begin(), string.end(), myChar);
}

size_t String::find(char myChar) {
  return string.find(myChar);
}

size_t String::size() {
  return string.size();
}

std::string String::substr(size_t startingIndex, size_t subStringSize) {
  return string.substr(startingIndex, subStringSize);
}

std::string_view String::get() {
  return string;
}
