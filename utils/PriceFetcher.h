#pragma once
#include <string>
#include <vector>
#include<deque>
#include <unordered_map>
#include <ctime>

class PricePoint{
public:
    double price;
    std::time_t timestamp;
    PricePoint() = default;
    PricePoint(double p, std::time_t t): price(p), timestamp(t){};
};

class PriceFetcher {
private:
    std::vector<std::string> symbols;
    std::unordered_map<std::string, double> prices;
    std::unordered_map<std::string, std::deque<PricePoint>> history;

public:
    PriceFetcher(const std::vector<std::string>& symbols);

    void updatePrices();  // updates existing prices
    double getPrice(const std::string& symbol); 
    std::string getRandomSymbol();
    void printPrices() const; // useful for debugging plus looks cool on CLI

    const std::deque<PricePoint>& getHistory(const std:: string& symbol) const;
};
