#include "PriceFetcher.h"
#include <random>

PriceFetcher::PriceFetcher(const std::vector<std::string>& symbols)
    : symbols(symbols) {}

std::unordered_map<std::string, double> PriceFetcher::generatePrices() {
    std::unordered_map<std::string, double> prices;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(50.0, 500.0);

    for (const auto& symbol : symbols) {
        prices[symbol] = dis(gen);
    }

    return prices;
}

std::string PriceFetcher::getRandomSymbol() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, symbols.size() - 1);
    return symbols[dis(gen)];
}

double PriceFetcher::getPrice(const std::string& symbol) {
    auto prices = generatePrices();
    return prices[symbol];
}
