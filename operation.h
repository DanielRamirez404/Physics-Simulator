#pragma once
#include "formula.h"
#include "usermath.h"
#include <cstddef>
#include <cassert>
#include <string>

namespace Math {
  template <typename T> class Operation {
  private:
    std::string formula{};
    size_t iterator{0};
    T result{};
    int numberOfOperations{};
    void parse();
    void solve();
    void solveFirstParenthesis();
    void solveByPriorities();
    int getNumberofOperations();
    T getNumber();
    void iterate();
    void iterateThroughNumber() { while (isNumeric(formula[iterator + 1])) iterate(); };
  public:
    Operation(const char* myFormula) : formula(myFormula) { parse(); };
    Operation(std::string myFormula) : formula(myFormula) { parse(); };
    T getResult() { return result; };
  };

  template <typename T> void Operation<T>::parse() {
    Formula::removeWhitespaces(formula);
    assert(!formula.empty() && "OPERATIONS CAN'T BE EMPTY");
    assert(Formula::areCharactersValid(formula) && "THERE SEEMS TO BE A NON-VALID CHARACTER AS INPUT");
    while (Formula::areThereParenthesis(formula)) solveFirstParenthesis();
    numberOfOperations = getNumberofOperations();
    solve();
  }

  template <typename T> void Operation<T>::solveFirstParenthesis() {
    Formula::assertParenthesisValidation(formula);
    constexpr int BothParenthesisCount {2};
    const size_t openingIndex{ Formula::getFirstParenthesisOpeningIndex(formula) };
    const size_t closingIndex{ Formula::getFirstParenthesisClosingIndex(formula) };
    const size_t afterOpeningIndex { openingIndex + 1 };
    const size_t afterClosingIndex { closingIndex + 1 };
    const size_t length{ afterClosingIndex - openingIndex };
    std::string parenthesisFormula{ formula.substr(afterOpeningIndex, length - BothParenthesisCount) };
    Operation internalOperation(parenthesisFormula);
    formula.replace( openingIndex, length, std::to_string( internalOperation.getResult() ) );
  }

  template <typename T> void Operation<T>::solveByPriorities() {
    //todo: make dedicated functions to make this function smaller

    //get MaxOrder
    int maxOrder{0};
    for (size_t i{0}; i < formula.size(); ++i) {
      if (isOperator(formula[i])) {
        if (Formula::isMinusSign(formula, i)) continue;
        if (maxOrder < getOperatorPriority(formula[i])) {
          ++maxOrder;
          if (maxOrder == maxOperatorPriority) break;
        } 
      }
    }
    // replace first highest order operation by parenthesis operation
    for (size_t i{0}; i < formula.size(); ++i) {
      if (isOperator(formula[i]) && (getOperatorPriority(formula[i]) == maxOrder)) {
        if (Formula::isMinusSign(formula, i)) continue;
        size_t j{1};
        while (isNumeric(formula[i - j])) {
          ++j;
          if ((i - j) + 1 == 0) break;
        }
        formula.insert(i - j + 1, "(");
        ++i;
        j = 1;
        while (isNumeric(formula[i + j]) || ((i + j) == formula.size() - 1)) ++j;
        ((i + j) == formula.size()) ? formula.append(")") : formula.insert(i + j + 1, ")");
        break;
      }
    }
    //solve parenthesis
    solveFirstParenthesis();
    --numberOfOperations;

    //self-explanatory
    (numberOfOperations == 1) ? solve() : solveByPriorities();
  }

  template <typename T> void Operation<T>::solve() {
    switch (numberOfOperations) {
      case 0:
        result = getNumber();
        break;
      case 1: 
      {
        T firstValue{getNumber()};
        iterate();
        char operation{formula[iterator]};
        iterate();
        T secondValue{getNumber()};
        result = doOperation<T>(firstValue, operation, secondValue);
        break;
      }
      default:
        solveByPriorities();
       break;
    }    
  }

  template <typename T> void Operation<T>::iterate() {
    assert((iterator + 1 < formula.size()) && "CAN\'T KEEP ITERATING");
    ++iterator;
  }

  template <typename T> int Operation<T>::getNumberofOperations() {
    int operatorCounter{0};
    for (size_t i{0}; i < formula.size(); ++i) {
      if (isOperator(formula[i])) {
        if (Formula::isMinusSign(formula, i)) continue;
        assert((i != 0) && (i != formula.size() - 1) && "OPERATORS CAN'T EITHER START NOR END FORMULAS");
        assert(isNumber(formula[i - 1]) && Formula::isPartOfNumber(formula, i + 1) && "OPERATORS MUST BE SURROUNDED BY NUMBERS");
        ++operatorCounter;
      }
    }
    return operatorCounter;
  }

  template <typename T> T Operation<T>::getNumber() {
    assert(Formula::isPartOfNumber(formula, iterator) && "OPERATIONS MUST START BY A NUMBER");
    const size_t firstDigit { iterator };
    iterateThroughNumber();
    const size_t firstNonDigit { iterator + 1};
    const size_t totalDigits { firstNonDigit - firstDigit };
    std::string numberString{ formula.substr(firstDigit, totalDigits) };
    return static_cast<T>( (Formula::isNumberDecimal(numberString)) ? std::stod(numberString) : std::stoi(numberString) );
  }
}
