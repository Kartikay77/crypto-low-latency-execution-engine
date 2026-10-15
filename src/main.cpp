#include "OrderBook.h"
#include "MarketDataReplay.h"
#include "ExecutionEngine.h"

#include <cstdint>
#include <deque>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <unordered_map>

int main()
{
    std::filesystem::create_directories("../data");

    std::ofstream file("../data/market_data.csv");

    if (!file.is_open())
    {
        std::cout << "Could not create market_data.csv\n";
        return 1;
    }

    file << "event,order_id,side,price,quantity\n";

    constexpr std::uint64_t TOTAL_EVENTS = 100000;

    std::uint64_t nextOrderId = 1;

    std::deque<std::uint64_t> liveOrderIds;
    std::unordered_map<std::uint64_t, Order> liveOrders;

    std::uint64_t addCount = 0;
    std::uint64_t cancelCount = 0;
    std::uint64_t modifyCount = 0;

    for (std::uint64_t i = 0; i < TOTAL_EVENTS; ++i)
    {
        int eventType = i % 10;

        // 70% ADD
        if (eventType < 7 || liveOrderIds.empty())
        {
            std::uint64_t orderId = nextOrderId++;

            Side side =
                (orderId % 2 == 0)
                ? Side::Buy
                : Side::Sell;

            double price;

            if (side == Side::Buy)
            {
                price = 90.0 + (orderId % 10);
            }
            else
            {
                price = 110.0 + (orderId % 10);
            }

            std::uint64_t quantity =
                1 + (orderId % 100);

            Order order{
                orderId,
                side,
                price,
                quantity,
                false
            };

            liveOrders[orderId] = order;
            liveOrderIds.push_back(orderId);

            file
                << "ADD,"
                << orderId
                << ","
                << ((side == Side::Buy) ? "BUY" : "SELL")
                << ","
                << price
                << ","
                << quantity
                << '\n';

            ++addCount;
        }

        // 20% CANCEL
        else if (eventType < 9)
        {
            std::uint64_t orderId =
                liveOrderIds.front();

            liveOrderIds.pop_front();

            Order order =
                liveOrders.at(orderId);

            file
                << "CANCEL,"
                << order.orderId
                << ","
                << ((order.side == Side::Buy) ? "BUY" : "SELL")
                << ","
                << order.price
                << ","
                << order.quantity
                << '\n';

            liveOrders.erase(orderId);

            ++cancelCount;
        }

        // 10% MODIFY
        else
        {
            std::uint64_t orderId =
                liveOrderIds.back();

            Order& order =
                liveOrders.at(orderId);

            order.quantity += 5;

            if (order.side == Side::Buy)
            {
                order.price =
                    90.0 + ((order.orderId + modifyCount) % 10);
            }
            else
            {
                order.price =
                    110.0 + ((order.orderId + modifyCount) % 10);
            }

            file
                << "MODIFY,"
                << order.orderId
                << ","
                << ((order.side == Side::Buy) ? "BUY" : "SELL")
                << ","
                << order.price
                << ","
                << order.quantity
                << '\n';

            ++modifyCount;
        }
    }

    file.close();

    std::cout << "market_data.csv created successfully\n\n";

    std::cout << "GENERATED EVENT MIX\n";
    std::cout << "Total Events: " << TOTAL_EVENTS << '\n';
    std::cout << "ADD: " << addCount << '\n';
    std::cout << "CANCEL: " << cancelCount << '\n';
    std::cout << "MODIFY: " << modifyCount << '\n';

    std::cout << "\nSTARTING MARKET DATA REPLAY...\n";

    OrderBook orderBook;
    MarketDataReplay replay;
    ExecutionEngine executionEngine;

    replay.replay(
        "../data/market_data.csv",
        orderBook,
        executionEngine
    );

    executionEngine.printStats();

    return 0;
}