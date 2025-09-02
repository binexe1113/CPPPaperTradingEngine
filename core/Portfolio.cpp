#include "Portfolio.h"
#include <iostream>
#include <stdexcept>

Portfolio::Portfolio(double initial_cash) : cash(initial_cash) {} //Constructor

void Portfolio::buyStock(const std::string& symbol, int sharesQtd, double pricePerShare){
    double cost = sharesQtd * pricePerShare;
    if (cost > cash){
        throw std::runtime_error("Insufficient funds to buy" + symbol);
    }

    cash -= cost;
    holdings[symbol] += sharesQtd;
    std::cout << "Bought " << sharesQtd << "shares of " << symbol
              << "at $" << pricePerShare << "each. Remaining cash: $" << cash << "\n"; 

    tradeLog.emplace_back(symbol,sharesQtd,pricePerShare,"BUY");
}

void Portfolio::sellStock(const std::string& symbol, int sharesQtd, double pricePerShare){
    if(holdings[symbol] < sharesQtd){
        throw std::runtime_error("Not enough shares of " + symbol + "to sell");
    }

    holdings[symbol] -= sharesQtd;
    cash += sharesQtd * pricePerShare;

    if (holdings[symbol] == 0){
        holdings.erase(symbol);
    }   

    tradeLog.emplace_back(symbol,sharesQtd,pricePerShare,"SELL");

}

void Portfolio::printPortfolio() const{
    std::cout << "\n === Portfolio ===\n";
    std::cout << "Cash: $" << cash << "\n";
    if (holdings.empty()){
        std::cout<< "No holdings.\n";
    }
    for (const auto&[symbol,sharesQtd] : holdings){
        std::cout<< symbol << ": " << sharesQtd << " shares\n";
    }
    
    std::cout<<"==================";
}

void Portfolio::printTradeLog() const{
    std::cout<<"\n === Trade Log ===\n";
        for (const auto& trade : tradeLog) {
        std::tm* tm = std::localtime(&trade.timestamp);
        char timeStr[20];
        std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", tm);

        std::cout << trade.type << " "
                  << trade.quantity << " " << trade.symbol
                  << " @ $" << std::fixed << std::setprecision(2) << trade.price
                  << " [" << timeStr << "]\n";
    }


}
