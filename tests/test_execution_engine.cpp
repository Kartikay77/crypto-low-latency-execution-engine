#include "OrderBook.h"
#include "ExecutionEngine.h"

#include <cassert>
#include <iostream>

void testBuyFill()
{
    OrderBook orderBook;
    ExecutionEngine executionEngine;

    // Our BUY order
    orderBook.addOrder(
        {1, Side::Buy, 105.0, 10, true}
    );

    // External SELL order
    orderBook.addOrder(
        {2, Side::Sell, 100.0, 4, false}
    );

    orderBook.matchOrders(
        [&executionEngine](const Trade& trade)
        {
            if (trade.buyIsOurOrder)
            {
                executionEngine.processBuyFill(trade);
            }

            if (trade.sellIsOurOrder)
            {
                executionEngine.processSellFill(trade);
            }
        }
    );

    assert(executionEngine.getPosition() == 4);
    assert(executionEngine.getCash() == -400.0);
    assert(executionEngine.getFillCount() == 1);

    std::cout << "testBuyFill PASSED\n";
}

void testSellFill()
{
    OrderBook orderBook;
    ExecutionEngine executionEngine;

    // External BUY order
    orderBook.addOrder(
        {3, Side::Buy, 105.0, 6, false}
    );

    // Our SELL order
    orderBook.addOrder(
        {4, Side::Sell, 100.0, 3, true}
    );

    orderBook.matchOrders(
        [&executionEngine](const Trade& trade)
        {
            if (trade.buyIsOurOrder)
            {
                executionEngine.processBuyFill(trade);
            }

            if (trade.sellIsOurOrder)
            {
                executionEngine.processSellFill(trade);
            }
        }
    );

    assert(executionEngine.getPosition() == -3);
    assert(executionEngine.getCash() == 300.0);
    assert(executionEngine.getFillCount() == 1);

    std::cout << "testSellFill PASSED\n";
}

void testPartialBuyFill()
{
    OrderBook orderBook;
    ExecutionEngine executionEngine;

    // Our BUY order for 10
    orderBook.addOrder(
        {5, Side::Buy, 105.0, 10, true}
    );

    // External SELL only for 3
    orderBook.addOrder(
        {6, Side::Sell, 100.0, 3, false}
    );

    orderBook.matchOrders(
        [&executionEngine](const Trade& trade)
        {
            if (trade.buyIsOurOrder)
            {
                executionEngine.processBuyFill(trade);
            }

            if (trade.sellIsOurOrder)
            {
                executionEngine.processSellFill(trade);
            }
        }
    );

    assert(executionEngine.getPosition() == 3);
    assert(executionEngine.getCash() == -300.0);
    assert(executionEngine.getFillCount() == 1);

    std::cout << "testPartialBuyFill PASSED\n";
}

int main()
{
    testBuyFill();
    testSellFill();
    testPartialBuyFill();

    std::cout << "\nALL EXECUTION ENGINE TESTS PASSED\n";

    return 0;
}