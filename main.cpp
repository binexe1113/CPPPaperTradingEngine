#include "core/portfolio.h"
#include "utils/PriceFetcher.h"
#include <iostream>
#include<thread>
#include<chrono>

int main() {

    std::vector<std::string> symbols = {"AAPL", "GOOG", "MSFT", "AMZN", "TSLA"}; //Set of random stocks JUST FOR TESTS
    std::string symbol;
    int quantity;
    double currentPrice;
    try {
        Portfolio portfolio(10000.0); // start with $10,000
        PriceFetcher fetcher(symbols);

            for (int i = 0; i < 10; ++i) {
        std::string symbol = fetcher.getRandomSymbol();
        double price = fetcher.getPrice(symbol);
        
        int quantity = rand() % 10 + 1; // buy/sell 1-10 shares
        bool buy = rand() % 2;          // randomly decide buy or sell (50%)

        try {
            if (buy) {
                portfolio.buyStock(symbol, quantity, price);
            } else {
                portfolio.sellStock(symbol, quantity, price);
            }
        } catch (const std::exception& e) {
            std::cout << "Trade failed: " << e.what() << "\n";
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // simulate time passing
    }

    // Print final portfolio and trade log
    portfolio.printPortfolio();
    portfolio.printTradeLog();

    return 0;

    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
}
