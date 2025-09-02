#include "threads/TradeWorker.h"
#include <iostream>

int main() {
    TradeWorker worker;

    std::cout << "Starting TradeWorker..." << std::endl;
    worker.start();

    std::this_thread::sleep_for(std::chrono::seconds(12));

    std::cout << "Stopping TradeWorker..." << std::endl;
    worker.stop();

    return 0;
}
