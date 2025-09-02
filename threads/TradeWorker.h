#pragma once
#include "../Portfolio.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <string>
#include <chrono>

class TradeWorker {
public:
    TradeWorker();
    ~TradeWorker();

    void start();
    void stop();

private:
    void timeUpdater();   // thread 1
    void buyer();         // thread 2

    std::thread timeThread;
    std::thread buyerThread;
    std::atomic<bool> running{false};

    std::mutex dataMutex;
    std::string currentTime;  // shared resource
};
