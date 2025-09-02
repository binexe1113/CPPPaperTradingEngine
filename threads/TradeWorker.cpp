#include "TradeWorker.h"
#include <iostream>
#include <ctime>

TradeWorker::TradeWorker() {}

TradeWorker::~TradeWorker() {
    stop();
}

void TradeWorker::start() {
    running = true;
    timeThread = std::thread(&TradeWorker::timeUpdater, this);
    buyerThread = std::thread(&TradeWorker::buyer, this);
}

void TradeWorker::stop() {
    running = false;
    if (timeThread.joinable()) timeThread.join();
    if (buyerThread.joinable()) buyerThread.join();
}

void TradeWorker::timeUpdater() {
    while (running) {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::time_t now = std::time(nullptr);
        std::string timeStr = std::ctime(&now);
        timeStr.pop_back(); // remove newline

        {
            std::lock_guard<std::mutex> lock(dataMutex);
            currentTime = timeStr;
        }

        std::cout << "[TimeUpdater] Time updated: " << timeStr << std::endl;
    }
}

void TradeWorker::buyer() {
    while (running) {
        std::this_thread::sleep_for(std::chrono::seconds(3));

        std::string localTime;
        {
            std::lock_guard<std::mutex> lock(dataMutex);
            localTime = currentTime;
        }

        std::cout << "[Buyer] Buying stocks at " << localTime << std::endl;
    }
}
