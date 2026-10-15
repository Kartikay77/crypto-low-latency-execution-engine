#pragma once
#include <cstdint>

struct Trade
{
    std::uint64_t buyOrderId;
    std::uint64_t sellOrderId;

    double price;
    std::uint64_t quantity;

    bool buyIsOurOrder;
    bool sellIsOurOrder;
};