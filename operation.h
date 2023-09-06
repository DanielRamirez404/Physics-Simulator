#pragma once
#include "usermath.h"
#include "userstring.h"
#include <cstddef>
#include <cassert>
#include <string>
#include <string_view>

namespace Math {
  template <typename T> class Operation {
  private:
    std::string formula{};
    size_t iterator{0};
    T firstValue{};
    T secondValue{};
    T result{};
    int numberOfOperations{};
    char operation{};
    void parse();
    bool areStringCharactersValid();
    void getData();
    void solve();
    void iterate();
    int getNumberofOperations();
    T getNumber();
    char getOperator();
    void iterateThroughNumber() { while (isNumeric(formula[iterator + 1])) iterate(); };
    bool isNumberDecimal(std::string_view numberString);
  public:
    Operation(const char* myFormula) : formula(myFormula) { parse(); };
    Operation(std::string& myFormula) : formula(myFormula) { parse(); };
    T getResult() { return result; };
  };

  template <typename T> void Operation<T>::parse() {
    String::eraseWhitespaces(formula);
    getData();
    solve();
  }

  template <typename T> void Operation<T>::solve() {
    switch (operation) {
      case '+':
        result = firstValue + secondValue;
        break;
      case '-':
        result = firstValue - secondValue;
        break;
      case '*':
        result = firstValue * secondValue;
        break;
      case '/':
        result = firstValue / secondValue;
        break;
      case '^':
        result = toThePower<T>(firstValue, static_cast<int>(secondValue));
        break;
    }
  }

  template <typename T> void Operation<T>::getData() {
    assert(areStringCharactersValid() && "THERE SEEMS TO BE A NON-VALID CHARACTER AS INPUT");
    numberOfOperations = getNumberofOperations();
    firstValue = getNumber();
    iterate();
    operation = getOperator();
    iterate();
    secondValue = getNumber();
  }

  template <typename T> void Operation<T>::iterate() {
    assert(((iterator + 1) < formula.size()) && "CAN\'T KEEP ITERATING");
    ++iterator;
  }

  template <typename T> bool Operation<T>::areStringCharactersValid() {
    for (size_t i{0}; i < formula.size(); ++i) {
      if (!isMathRelated(formula[i])) return false;
    }
    return true;
  }

  template <typename T> int Operation<T>::getNumberofOperations() {
    int operatorCounter{0};
    for (size_t i{0}; i < formula.size(); ++i) {
      if (isOperator(formula[i])) {
        assert((i != formula.size() - 1) && "OPERATORS CAN'T END FORMULAS");
        if (formula[i] == '-' && (i == 0 || isOperator(formula[i - 1]))) {  //if isn't substraction operator
          assert(isNumber(formula[i + 1]) && "THERE MUST BE A NUMBER AFTER THE MINUS SIGN");
          continue;
        }
        assert((i != 0) && (i != formula.size() - 1) && "OPERATORS CAN'T START FORMULAS");
        assert(isNumber(formula[i - 1]) 
              && (isNumber(formula[i + 1]) || 
              (formula[i + 1] == '-' && (isNumber(formula[i + 2]))))
              && "OPERATORS MUST BE SURROUNDED BY NUMBERS");
        ++operatorCounter;
      }
    }
    return operatorCounter;
  }

  template <typename T> T Operation<T>::getNumber() {
    assert((isNumber(formula[iterator]) || formula[iterator] == '-')  && "OPERATIONS MUST START BY A NUMBER");
    const size_t firstDigit { iterator };
    iterateThroughNumber();
    const size_t firstNonDigit { iterator + 1};
    const size_t totalDigits { firstNonDigit - firstDigit };
    std::string numberString{ formula.substr(firstDigit, totalDigits) };
    bool isDecimal { isNumberDecimal(numberString) };
    return static_cast<T>( (isDecimal) ? std::stod(numberString) : std::stoi(numberString) );
  }

  template <typename T> char Operation<T>::getOperator() {
    assert(isOperator(formula[iterator]));
    return formula[iterator];
  }

  template <typename T> bool Operation<T>::isNumberDecimal(std::string_view numberString) {
    int pointCounter{0};
    for (size_t i{0}; i < numberString.size(); ++i) {
      if (numberString[i] == '.') {
      assert((pointCounter <= 1) && "NUMBERS CAN\'T HAVE MORE THAN ONE DECIMAL POINT");
      assert(isNumber(numberString[i + 1]) && "THERE MUST BE A NUMBER AFTER A DECIMAL POINT");
      ++pointCounter;
      }
    }
    return true;
  }
}
