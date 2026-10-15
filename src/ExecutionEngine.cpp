#include "ExecutionEngine.h"
#include <iostream>

void ExecutionEngine::processBuyFill(const Trade& trade) {
    position += trade.quantity;
    cash -= trade.price * static_cast<double>(trade.quantity);
    ++fillCount;
}

void ExecutionEngine::processSellFill(const Trade& trade){
    position -= trade.quantity;
    cash += trade.price * static_cast<double>(trade.quantity);
    ++fillCount;
}

std::int64_t ExecutionEngine::getPosition() const {
    return position;
}

double ExecutionEngine::getCash() const {
    return cash;
}

std::uint64_t ExecutionEngine::getFillCount() const {
    return fillCount;
}

void ExecutionEngine::printStats() const {
    std::cout << "Execution Engine Stats:\n";
    std::cout << "Position: " << position << "\n";
    std::cout << "Cash: " << cash << "\n";
    std::cout << "Fill Count: " << fillCount << "\n";
}

