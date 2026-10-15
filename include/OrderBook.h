#pragma once
#include "Order.h"
#include "Trade.h"
#include <map>
#include <deque>
#include <functional>



class OrderBook{
    private:
        std::map<double, std::deque<Order>, std::greater<double>> bids;
        std::map<double, std::deque<Order>> asks;
    public:
        void addOrder(const Order& order);
        double bestBid() const;
        double bestAsk() const;
        void printOrderBook() const;
        void matchOrders(std::function<void(const Trade&)> callback);
        void modifyOrder(const Order& oldOrder, const Order& newOrder);
        bool removeOrder(std::uint64_t orderId);
};