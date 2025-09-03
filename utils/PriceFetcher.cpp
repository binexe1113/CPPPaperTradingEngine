#include "PriceFetcher.h"
#include <random>
#include <iostream>

PriceFetcher::PriceFetcher(const std::vector<std::string>& symbols)
    : symbols(symbols) 
{
    // Initialize prices once
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(50.0, 500.0);

    for (const auto& symbol : symbols) {
        prices[symbol] = dis(gen);
    }
}

void PriceFetcher::updatePrices() {
    for (auto& [symbol, price] : prices) {
        double change = ((rand() % 2001) - 1000) / 100.0; // random change [-10, +10]
        price += change;
        if (price < 1.0) price = 1.0; // avoid negative/zero prices
    }
}

double PriceFetcher::getPrice(const std::string& symbol) {
    if (prices.find(symbol) != prices.end()) {
        return prices[symbol];
    }
    throw std::runtime_error("Symbol not found: " + symbol);
}

std::string PriceFetcher::getRandomSymbol() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, symbols.size() - 1);
    return symbols[dis(gen)];
}

void PriceFetcher::printPrices() const {
    for (const auto& [symbol, price] : prices) {
        std::cout << symbol << ": $" << price << "\n";
    }
}
