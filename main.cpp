#include "core/portfolio.h"
#include <iostream>

int main() {
    std::string symbol;
    int quantity;
    double currentPrice;
    try {
        Portfolio portfolio(10000.0); // start with $10,000

        std::cout<<"Enter ticker symbol";
        std::cin >> symbol;

        std::cout << "Enter the quantity";
        std::cin >> quantity;

        std::cout<< "Enter the price";
        std::cin >> currentPrice;

        portfolio.buyStock(symbol, quantity, currentPrice);
        portfolio.printPortfolio();

        std::cout << "Lets sell";
        system("pause");
        portfolio.sellStock(symbol,quantity,currentPrice);
        portfolio.printPortfolio();

        portfolio.printTradeLog(); 

    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
}
