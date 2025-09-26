// YahooFinanceAPI.cpp
#include "yhfinance.h"
#include <iostream>
#include <iostream>
#include<thread>
#include <chrono>
// Include your HTTP library, e.g., cURL or cpp-httplib
// #include <httplib.h>  

double YahooFinanceAPI::getPrice(const std::string& symbol) {
    std::string url = "https://query1.finance.yahoo.com/v7/finance/quote?symbols=" + symbol;
    std::string jsonData = fetchData(url);
    return parsePrice(jsonData);
}

std::string YahooFinanceAPI::fetchData(const std::string& url) {
    // Placeholder: implement HTTP GET request here
    // Return the JSON response as a string
    std::cout << "Fetching data from: " << url << std::endl;
    return "{}"; // temporary dummy
}

double YahooFinanceAPI::parsePrice(const std::string& jsonData) {
    // Placeholder: parse JSON and return the price
    // e.g., use nlohmann::json or similar
    return 0.0; // temporary dummy
}

// Continuous updater
void YahooFinanceAPI::priceUpdater(const std::vector<std::string>& symbols, int intervalSeconds) {
    while (true) {
        for (const auto& symbol : symbols) {
            double price = getPrice(symbol);
            std::cout << symbol << ": " << price << std::endl; // or update your UI directly
        }
        std::this_thread::sleep_for(std::chrono::seconds(intervalSeconds));
    }
}
