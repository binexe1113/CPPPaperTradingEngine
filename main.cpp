#include "core/portfolio.h"
#include <iostream>

int main() {
    try {
        Portfolio portfolio(10000.0); // start with $10,000

        portfolio.buyStock("AAPL", 10, 150.0);
        portfolio.buyStock("MSFT", 5, 200.0);
        portfolio.sellStock("AAPL", 5, 155.0);
        portfolio.printPortfolio();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
}
