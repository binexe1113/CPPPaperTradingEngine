#include "core/portfolio.h"
#include "utils/PriceFetcher.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <cstdlib>

std::atomic<bool> running(true); // control flag for stopping threads

void priceUpdater(PriceFetcher& fetcher) {
    while (running) {
        fetcher.updatePrices(); // You need to implement this inside PriceFetcher
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void userInputHandler(Portfolio& portfolio, PriceFetcher& fetcher) {
    std::string symbol;
    int quantity;
    std::string action;

    while (running) {
        std::cout << "\nEnter command (buy/sell symbol quantity) or 'quit': ";
        std::cin >> action;
        if (action == "quit") {
            running = false;
            break;
        }
        std::cin >> symbol >> quantity;

        try {
            double price = fetcher.getPrice(symbol);
            if (action == "buy") {
                portfolio.buyStock(symbol, quantity, price);
            } else if (action == "sell") {
                portfolio.sellStock(symbol, quantity, price);
            } else {
                std::cout << "Invalid action.\n";
            }
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
        }
    }
}

int main() {
    try {
        std::vector<std::string> symbols = {"AAPL", "GOOG", "MSFT", "AMZN", "TSLA"};
        Portfolio portfolio(10000.0); // start with $10,000
        PriceFetcher fetcher(symbols);

        // Start threads
        std::thread priceThread(priceUpdater, std::ref(fetcher));
        std::thread inputThread(userInputHandler, std::ref(portfolio), std::ref(fetcher));

        // Wait for threads
        inputThread.join();
        running = false; // stop price thread after user quits
        priceThread.join();

        // Print final portfolio and trade log
        portfolio.printPortfolio();
        portfolio.printTradeLog();

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Fatal Error: " << e.what() << "\n";
    }
}
