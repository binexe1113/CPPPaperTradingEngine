#pragma once
#include <unordered_map>
#include <string>
#include<vector>
#include "Trade.h"

struct Portfolio {
private:
    double cash;
    std::unordered_map<std::string, int> holdings;
    std::vector<Trade> tradeLog;

public:
    Portfolio(double initial_cash);

    void buyStock(const std::string& symbol, int sharesQtd, double pricePerShare);
    void sellStock(const std::string& symbol, int sharesQtd, double pricePerShare);
    void printPortfolio() const;
    void printTradeLog() const;
};