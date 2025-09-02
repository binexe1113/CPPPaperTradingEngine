#pragma once
#include <string>
#include <vector>
#include <unordered_map>

class PriceFetcher {
public:
    PriceFetcher(const std::vector<std::string>& symbols);

    std::unordered_map<std::string, double> generatePrices(); 
    std::string getRandomSymbol();
    double getPrice(const std::string& symbol);

private:
    std::vector<std::string> symbols;
};
