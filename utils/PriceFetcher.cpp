#include "PriceFetcher.h"
#include <random>
#include <iostream>

PriceFetcher::PriceFetcher(const std::vector<std::string>& symbols)
    : symbols(symbols)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(50.0, 500.0);

    for (const auto& symbol : symbols) {
        prices[symbol] = dis(gen);
        history[symbol].push_back( PricePoint(prices[symbol], std::time(nullptr)) );
    }
}


void PriceFetcher::updatePrices() {
    for (auto& [symbol, price] : prices) {
        double change = ((rand() % 2001) - 1000) / 100.0; // random change [-10, +10]
        price += change;
        if (price < 1.0) price = 1.0; // avoid negative/zero prices

        // store history (now works because PricePoint(double, time_t) exists)
        PricePoint p(price, std::time(nullptr));
        auto& hist = history[symbol];
        hist.push_back(p);
        if (hist.size() > 500) hist.pop_front(); // keep last 500 points
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




