#include "TiingoFinance.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib> // getenv
#include "../external/json/single_include/nlohmann/json.hpp"
#include <curl/curl.h>

using json = nlohmann::json;

static size_t CurlWriteCallback(void* ptr, size_t size, size_t nmemb, void* userdata) {
    auto* out = static_cast<std::string*>(userdata);
    out->append(static_cast<char*>(ptr), size * nmemb);
    return size * nmemb;
}

// Get API key from environment variable
std::string TiingoFinanceAPI::getApiKey() {
    const char* key = std::getenv("TIINGO_API_KEY");
    if (!key || key[0] == '\0') {
        std::cerr << "TIINGO_API_KEY not set in environment.\n";
        return "";
    }
    return std::string(key);
}

// Fetch JSON data from Tiingo
std::string TiingoFinanceAPI::fetchData(const std::string& symbol) {
    std::string apiKey = getApiKey();
    if (apiKey.empty()) return {};

    std::string url = "https://api.tiingo.com/tiingo/daily/" + symbol + "/prices?token=" + apiKey;

    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "curl_easy_init() failed\n";
        return {};
    }

    std::string response;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlWriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0");
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 15L);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "curl error: " << curl_easy_strerror(res) << " for URL: " << url << std::endl;
        response.clear();
    } else {
        long http_code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
        if (http_code >= 400) {
            std::cerr << "HTTP error: " << http_code << " for URL: " << url << std::endl;
            response.clear();
        }
    }

    curl_easy_cleanup(curl);
    return response;
}

// Parse price from Tiingo JSON
double TiingoFinanceAPI::parsePrice(const std::string& jsonData) {
    try {
        auto j = json::parse(jsonData);
        if (!j.empty() && j[0].contains("close")) {
            return j[0]["close"].get<double>();
        }
    } catch (...) {
        return 0.0;
    }
    return 0.0;
}

// Get price for a single ticker
double TiingoFinanceAPI::getPrice(const std::string& symbol) {
    std::string data = fetchData(symbol);
    return parsePrice(data);
}

// Continuous updater (interval in seconds)
void TiingoFinanceAPI::priceUpdater(const std::vector<std::string>& symbols, int intervalSeconds) {
    while (true) {
        for (const auto& symbol : symbols) {
            double price = getPrice(symbol);
            std::cout << symbol << ": " << price << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(intervalSeconds));
    }
}
