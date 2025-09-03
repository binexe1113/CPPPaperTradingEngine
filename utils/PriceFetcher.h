#pragma once
#include <string>
#include <vector>
#include <unordered_map>

class PriceFetcher {
private:
    std::vector<std::string> symbols;
    std::unordered_map<std::string, double> prices;

public:
    PriceFetcher(const std::vector<std::string>& symbols);

    void updatePrices();  // updates existing prices
    double getPrice(const std::string& symbol); 
    std::string getRandomSymbol();
    void printPrices() const; // optional, useful for debugging
};
