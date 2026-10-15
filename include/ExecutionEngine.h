#pragma once
#include "Trade.h"
#include <cstdint>

class ExecutionEngine {
private:
    // Private members can be added here if needed
    std::int16_t position = 0; // Example private member
    double cash = 0.0; // Example private member
    std::uint64_t fillCount = 0; // Example private member
    
public:
    void processBuyFill(const Trade& trade);
    void processSellFill(const Trade& trade);
    std::int64_t getPosition() const;
    double getCash() const;
    std::uint64_t getFillCount() const;
    void printStats() const;
};