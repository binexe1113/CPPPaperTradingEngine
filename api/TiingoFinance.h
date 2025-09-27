#pragma once
#include <vector>
#include <string>

class YahooFinanceAPI {
public:
    YahooFinanceAPI() = default;

    // Fetch the latest price for a given ticker symbol
    double getPrice(const std::string& symbol);
    
    // Start a continuous price updater (blocking function)
    void priceUpdater(const std::vector<std::string>& symbols, int intervalSeconds = 5);


private:
    // Optional: helper methods for fetching/parsing JSON from Yahoo Finance
    std::string fetchData(const std::string& url);
    double parsePrice(const std::string& jsonData);
};
