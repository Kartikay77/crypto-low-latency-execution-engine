#pragma once

#include "OrderBook.h"
#include "ExecutionEngine.h"
#include <string>

class MarketDataReplay
{
public:
    void replay(const std::string& filePath, OrderBook& orderBook, ExecutionEngine& executionEngine);
};