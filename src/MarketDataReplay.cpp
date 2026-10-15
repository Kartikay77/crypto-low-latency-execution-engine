#include "MarketDataReplay.h"

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

void MarketDataReplay::replay(
    const std::string& filePath,
    OrderBook& orderBook,
    ExecutionEngine& executionEngine)
{
    std::ifstream file(filePath);

    if (!file.is_open())
    {
        std::cout << "Could not open market data file\n";
        return;
    }

    std::string line;
    std::vector<long long> latencies;

    std::getline(file, line); // skip header

    auto totalStart =
        std::chrono::steady_clock::now();

    while (std::getline(file, line))
    {
        auto start =
            std::chrono::steady_clock::now();

        std::stringstream ss(line);

        std::string event;
        std::string orderIdString;
        std::string sideString;
        std::string priceString;
        std::string quantityString;

        std::getline(ss, event, ',');
        std::getline(ss, orderIdString, ',');
        std::getline(ss, sideString, ',');
        std::getline(ss, priceString, ',');
        std::getline(ss, quantityString, ',');

        std::uint64_t orderId =
            std::stoull(orderIdString);

        double price =
            std::stod(priceString);

        std::uint64_t quantity =
            std::stoull(quantityString);

        Side side =
            (sideString == "BUY")
            ? Side::Buy
            : Side::Sell;

        Order order{
            orderId,
            side,
            price,
            quantity
        };

        if (event == "ADD")
        {
            orderBook.addOrder(order);
        }
        else if (event == "CANCEL")
        {
            orderBook.removeOrder(orderId);
        }
        else if (event == "MODIFY")
        {
            orderBook.removeOrder(orderId);
            orderBook.addOrder(order);
        }

        orderBook.matchOrders(
            [&executionEngine](const Trade& trade)
            {
                if (trade.buyIsOurOrder){
                    executionEngine.processBuyFill(trade);
                }
                else if (trade.sellIsOurOrder)
                {
                    executionEngine.processSellFill(trade);
                }
            }
        );

        auto end =
            std::chrono::steady_clock::now();

        auto latency =
            std::chrono::duration_cast<
                std::chrono::nanoseconds>(
                    end - start
                ).count();

        latencies.push_back(latency);
    }

    auto totalEnd =
        std::chrono::steady_clock::now();

    if (!latencies.empty())
    {
        std::sort(
            latencies.begin(),
            latencies.end()
        );

        long long total = 0;

        for (long long latency : latencies)
        {
            total += latency;
        }

        double average =
            static_cast<double>(total)
            / latencies.size();

        auto percentile =
            [&](double p)
            {
                std::size_t index =
                    static_cast<std::size_t>(
                        p * (latencies.size() - 1)
                    );

                return latencies[index];
            };

        double totalSeconds =
            std::chrono::duration<double>(
                totalEnd - totalStart
            ).count();

        double eventsPerSecond =
            latencies.size() / totalSeconds;

        std::cout << "\nLATENCY STATS\n";

        std::cout
            << "Events: "
            << latencies.size()
            << '\n';

        std::cout
            << "Average: "
            << average
            << " ns\n";

        std::cout
            << "P50: "
            << percentile(0.50)
            << " ns\n";

        std::cout
            << "P95: "
            << percentile(0.95)
            << " ns\n";

        std::cout
            << "P99: "
            << percentile(0.99)
            << " ns\n";

        std::cout
            << "Total Time: "
            << totalSeconds
            << " seconds\n";

        std::cout
            << "Throughput: "
            << eventsPerSecond
            << " events/second\n";
    }
}