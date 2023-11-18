#pragma once

class Variable {
private:
  const char identifier{};
  float value{};
  bool hasBeenSet{false};
  bool isSigned{true};
public:
  Variable(char myIdentifier, float myValue) : identifier{myIdentifier}, value{myValue} {};
  explicit Variable(char myIdentifier) : identifier{myIdentifier} {};
  void setSignedness(bool myBool);
  void set(float myValue);
  float get() const;
  bool isSet() const;
  char getIdentifier() const;
};
