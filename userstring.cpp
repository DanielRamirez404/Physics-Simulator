#include "userstring.h"
#include <cstddef>
#include <string>
#include <string_view>
#include <algorithm>

void String::add(std::string_view myString, size_t index) {
  (index >= string.size()) ? string.append(myString) : string.insert(index, myString);
}

void String::add(char charToAdd, size_t index) {
  if (index >= string.size())
    string += charToAdd;
  else
    string.insert(index, 1, charToAdd);
}

void String::prepend(std::string_view myString) {
  string.insert(0, myString);
}

void String::prepend(char myChar) {
  string.insert(0, 1, myChar);
}

void String::append(std::string_view myString) {
  string.append(myString);
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

std::string String::cut(size_t startingIndex, size_t cutStringSize) {
  std::string cutString { string.substr(startingIndex, cutStringSize) };
  string.erase(startingIndex, cutStringSize);
  return cutString;
}

char String::cut(size_t index) {
  char cutChar { string[index] };
  string.erase(index, 1);
  return cutChar;
}

std::string_view String::get() {
  return string;
}
