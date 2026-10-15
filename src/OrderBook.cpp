#include "OrderBook.h"
#include <iostream>
#include <algorithm>

void OrderBook::addOrder(const Order& order) {
    if (order.side == Side::Buy) {
        bids[order.price].push_back(order);
    } else {
        asks[order.price].push_back(order);
    }
}

double OrderBook::bestBid() const {
    if (bids.empty()) {
        return 0.0;
    }
    return bids.begin()->first;
}

double OrderBook::bestAsk() const {
    if (asks.empty()) {
        return 0.0;
    }
    return asks.begin()->first;
}

void OrderBook::printOrderBook() const {
    std::cout << "Order Book:" << std::endl;
    std::cout << "Bids:" << std::endl;
    for (const auto& [price, orders] : bids) {
        std::uint64_t totalQuantity = 0;
        for (const auto& order : orders) {
            totalQuantity += order.quantity;
            std::cout << "Order ID: " << order.orderId << ", Price: " << price << ", Quantity: " << order.quantity << std::endl;
        }
        std::cout << price << " -> " << totalQuantity << '\n';
    }
    std::cout << "Asks:" << std::endl;
    for (const auto& [price, orders] : asks) {
        std::uint64_t totalQuantity = 0;
        for (const auto& order : orders) {
            totalQuantity += order.quantity;
            std::cout << "Order ID: " << order.orderId << ", Price: " << price << ", Quantity: " << order.quantity << std::endl;
        }
        std::cout << "Total Quantity at Price " << price << ": " << totalQuantity << std::endl;
    }
}

void OrderBook::matchOrders(
    std::function<void(const Trade&)> callback)
{
    while (!bids.empty() && !asks.empty())
    {
        auto bestBidIt = bids.begin();
        auto bestAskIt = asks.begin();

        double bestBidPrice = bestBidIt->first;
        double bestAskPrice = bestAskIt->first;

        if (bestBidPrice < bestAskPrice)
        {
            break;
        }

        auto& bidOrders = bestBidIt->second;
        auto& askOrders = bestAskIt->second;

        Order& bidOrder = bidOrders.front();
        Order& askOrder = askOrders.front();

        std::uint64_t matchedQuantity =
            std::min(bidOrder.quantity, askOrder.quantity);

        Trade trade{
            bidOrder.orderId,
            askOrder.orderId,
            bestAskPrice,
            matchedQuantity,
            bidOrder.isOurOrder,
            askOrder.isOurOrder
        };

        callback(trade);

        bidOrder.quantity -= matchedQuantity;
        askOrder.quantity -= matchedQuantity;

        if (bidOrder.quantity == 0)
        {
            bidOrders.pop_front();
        }

        if (askOrder.quantity == 0)
        {
            askOrders.pop_front();
        }

        if (bidOrders.empty())
        {
            bids.erase(bestBidIt);
        }

        if (askOrders.empty())
        {
            asks.erase(bestAskIt);
        }
    }
}

bool OrderBook::removeOrder(std::uint64_t orderId)
{
    for (auto bidIt = bids.begin(); bidIt != bids.end(); ++bidIt)
    {
        auto& orders = bidIt->second;

        auto orderIt = std::find_if(
            orders.begin(),
            orders.end(),
            [orderId](const Order& order)
            {
                return order.orderId == orderId;
            }
        );

        if (orderIt != orders.end())
        {
            orders.erase(orderIt);

            if (orders.empty())
            {
                bids.erase(bidIt);
            }

            return true;
        }
    }

    for (auto askIt = asks.begin(); askIt != asks.end(); ++askIt)
    {
        auto& orders = askIt->second;

        auto orderIt = std::find_if(
            orders.begin(),
            orders.end(),
            [orderId](const Order& order)
            {
                return order.orderId == orderId;
            }
        );

        if (orderIt != orders.end())
        {
            orders.erase(orderIt);

            if (orders.empty())
            {
                asks.erase(askIt);
            }

            return true;
        }
    }

    return false;
}

void OrderBook::modifyOrder(const Order& oldOrder, const Order& newOrder) {
    removeOrder(oldOrder.orderId);
    addOrder(newOrder);
}