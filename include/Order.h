#pragma once
#include <cstdint>

enum class Side : uint8_t
{
    Buy,
    Sell
};

struct Order
{
    std::uint64_t orderId;
    Side side;
    double price;
    std::uint64_t quantity;
    bool isOurOrder = false;
};