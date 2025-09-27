#include "PriceFetcher.h"
#include "TiingoFinance.h" // use the Tiingo API class
#include <iostream>
#include <thread>
#include <chrono>

PriceFetcher::PriceFetcher(const std::vector<std::string>& symbols)
    : symbols(symbols)
{
    // Initialize prices by fetching from Tiingo
    TiingoFinanceAPI api;

    for (const auto& symbol : symbols) {
        double price = api.getPrice(symbol);
        if (price <= 0.0) price = 1.0; // fallback
        prices[symbol] = price;
        history[symbol].push_back(PricePoint(price, std::time(nullptr)));
    }
}

// Update prices by fetching new data from Tiingo
void PriceFetcher::updatePrices() {
    TiingoFinanceAPI api;

    for (auto& [symbol, price] : prices) {
        double newPrice = api.getPrice(symbol);
        if (newPrice <= 0.0) newPrice = price; // fallback to last known

        price = newPrice;

        PricePoint p(price, std::time(nullptr));
        auto& hist = history[symbol];
        hist.push_back(p);
        if (hist.size() > 500) hist.pop_front();
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

const std::deque<PricePoint>& PriceFetcher::getHistory(const std::string& symbol) const {
    static std::deque<PricePoint> empty;
    if (history.find(symbol) != history.end()) {
        return history.at(symbol);
    }
    return empty;
}
