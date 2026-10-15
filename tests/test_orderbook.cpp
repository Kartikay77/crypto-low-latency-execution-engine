#include "OrderBook.h"

#include <cassert>
#include <iostream>
#include <vector>

void testBestBidAsk()
{
    OrderBook book;

    book.addOrder({1, Side::Buy, 100.0, 5});
    book.addOrder({2, Side::Buy, 101.0, 5});
    book.addOrder({3, Side::Sell, 103.0, 5});
    book.addOrder({4, Side::Sell, 102.0, 5});

    assert(book.bestBid() == 101.0);
    assert(book.bestAsk() == 102.0);

    std::cout << "testBestBidAsk PASSED\n";
}

void testPartialFill()
{
    OrderBook book;

    std::uint64_t matchedQuantity = 0;

    book.addOrder({1, Side::Buy, 100.0, 10});
    book.addOrder({2, Side::Sell, 99.0, 4});

    book.matchOrders(
        [&](const Trade& trade)
        {
            matchedQuantity += trade.quantity;
        }
    );

    assert(matchedQuantity == 4);
    assert(book.bestBid() == 100.0);
    assert(book.bestAsk() == 0.0);

    std::cout << "testPartialFill PASSED\n";
}

void testMultipleFills()
{
    OrderBook book;

    std::uint64_t totalMatched = 0;

    book.addOrder({1, Side::Buy, 100.0, 10});
    book.addOrder({2, Side::Sell, 98.0, 3});
    book.addOrder({3, Side::Sell, 99.0, 4});

    book.matchOrders(
        [&](const Trade& trade)
        {
            totalMatched += trade.quantity;
        }
    );

    assert(totalMatched == 7);
    assert(book.bestBid() == 100.0);

    std::cout << "testMultipleFills PASSED\n";
}

void testFIFO()
{
    OrderBook book;

    std::vector<std::uint64_t> sellOrderIds;

    book.addOrder({1, Side::Sell, 100.0, 5});
    book.addOrder({2, Side::Sell, 100.0, 5});
    book.addOrder({3, Side::Buy, 100.0, 7});

    book.matchOrders(
        [&](const Trade& trade)
        {
            sellOrderIds.push_back(trade.sellOrderId);
        }
    );

    assert(sellOrderIds.size() == 2);
    assert(sellOrderIds[0] == 1);
    assert(sellOrderIds[1] == 2);

    std::cout << "testFIFO PASSED\n";
}

void testCancel()
{
    OrderBook book;

    book.addOrder({1, Side::Buy, 100.0, 5});
    book.addOrder({2, Side::Buy, 99.0, 5});

    bool removed = book.removeOrder(1);

    assert(removed);
    assert(book.bestBid() == 99.0);

    std::cout << "testCancel PASSED\n";
}

void testModify()
{
    OrderBook book;

    Order oldOrder{
        1,
        Side::Buy,
        100.0,
        5
    };

    Order newOrder{
        1,
        Side::Buy,
        102.0,
        8
    };

    book.addOrder(oldOrder);

    book.modifyOrder(
        oldOrder,
        newOrder
    );

    assert(book.bestBid() == 102.0);

    std::cout << "testModify PASSED\n";
}

int main()
{
    testBestBidAsk();
    testPartialFill();
    testMultipleFills();
    testFIFO();
    testCancel();
    testModify();

    std::cout << "\nALL ORDER BOOK TESTS PASSED\n";

    return 0;
}