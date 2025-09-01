#pragma once
#include <unordered_map>
#include <string>

struct Portfolio {
    double cash;
    std::unordered_map<std::string, int> holdings;

    Portfolio(double initial_cash);

    void buyStock(const std::string& symbol, int shares, double pricePerShare);
    void sellStock(const std::string& symbol, int shares, double pricePerShare);
    void printPortfolio() const;
};