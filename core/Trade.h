#pragma once
#include<string>
#include<chrono>
#include<ctime>

struct Trade{
    std::string symbol;
    int quantity;
    double price;
    std::string type; //BUY or SELL
    std::time_t timestamp;

    Trade(const std::string& s,int q, double p, const std::string t)
        : symbol(s), quantity(q), price(p), type(t), timestamp(std::time(nullptr)) {}
    
};