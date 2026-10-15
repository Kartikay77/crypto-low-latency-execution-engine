#include "RiskEngine.h"

#include <cassert>
#include <iostream>

void testValidOrder()
{
    RiskEngine riskEngine(
        100,
        50,
        10000.0
    );

    Order order{
        1,
        Side::Buy,
        100.0,
        20,
        true
    };

    assert(
        riskEngine.validateOrder(
            order,
            0
        )
    );

    std::cout << "testValidOrder PASSED\n";
}

void testMaxOrderQuantity()
{
    RiskEngine riskEngine(
        100,
        50,
        10000.0
    );

    Order order{
        2,
        Side::Buy,
        100.0,
        60,
        true
    };

    assert(
        !riskEngine.validateOrder(
            order,
            0
        )
    );

    std::cout
        << "testMaxOrderQuantity PASSED\n";
}

void testMaxPosition()
{
    RiskEngine riskEngine(
        100,
        50,
        10000.0
    );

    Order order{
        3,
        Side::Buy,
        100.0,
        20,
        true
    };

    assert(
        !riskEngine.validateOrder(
            order,
            90
        )
    );

    std::cout << "testMaxPosition PASSED\n";
}

void testMaxNotional()
{
    RiskEngine riskEngine(
        100,
        50,
        10000.0
    );

    Order order{
        4,
        Side::Buy,
        1000.0,
        20,
        true
    };

    assert(
        !riskEngine.validateOrder(
            order,
            0
        )
    );

    std::cout << "testMaxNotional PASSED\n";
}

void testKillSwitch()
{
    RiskEngine riskEngine(
        100,
        50,
        10000.0
    );

    Order order{
        5,
        Side::Buy,
        100.0,
        10,
        true
    };

    riskEngine.activateKillSwitch();

    assert(
        !riskEngine.validateOrder(
            order,
            0
        )
    );

    riskEngine.deactivateKillSwitch();

    assert(
        riskEngine.validateOrder(
            order,
            0
        )
    );

    std::cout << "testKillSwitch PASSED\n";
}

int main()
{
    testValidOrder();
    testMaxOrderQuantity();
    testMaxPosition();
    testMaxNotional();
    testKillSwitch();

    std::cout
        << "\nALL RISK ENGINE TESTS PASSED\n";

    return 0;
}