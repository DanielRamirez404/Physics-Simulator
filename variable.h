#pragma once

class Variable {
private:
  const char identifier{};
  float value{};
  bool hasBeenSet{false};
  bool isSigned{true};
public:
  Variable(char myIdentifier, float myValue) : identifier(myIdentifier), value(myValue) {};
  Variable(char myIdentifier) : identifier(myIdentifier) {};
  void setSignedness(bool myBool);
  void set(float myValue);
  float get();
  bool isSet();
  char getIdentifier();
};
