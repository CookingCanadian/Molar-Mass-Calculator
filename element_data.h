#pragma once
#include <string>
#include <vector>

struct Element {
    int atomicNumber;
    std::string symbol;
    std::string name;
    double molarMass;
};

const std::vector<Element>& GetPeriodicTable();

double CalculateMolarMass(const std::string& formula);

const Element* FindElementBySymbol(const std::string& symbol);
