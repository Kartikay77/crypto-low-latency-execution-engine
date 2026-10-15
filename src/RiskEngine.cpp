#include "RiskEngine.h"

#include <cmath>

RiskEngine::RiskEngine(
    std::int64_t maxPosition,
    std::uint64_t maxOrderQuantity,
    double maxNotional)
    : maxPosition(maxPosition),
      maxOrderQuantity(maxOrderQuantity),
      maxNotional(maxNotional),
      killSwitchActive(false)
{
}

bool RiskEngine::validateOrder(
    const Order& order,
    std::int64_t currentPosition) const
{
    if (killSwitchActive)
    {
        return false;
    }

    if (order.quantity > maxOrderQuantity)
    {
        return false;
    }

    double orderNotional =
        order.price *
        static_cast<double>(order.quantity);

    if (orderNotional > maxNotional)
    {
        return false;
    }

    std::int64_t projectedPosition =
        currentPosition;

    if (order.side == Side::Buy)
    {
        projectedPosition +=
            static_cast<std::int64_t>(
                order.quantity
            );
    }
    else
    {
        projectedPosition -=
            static_cast<std::int64_t>(
                order.quantity
            );
    }

    if (std::abs(projectedPosition) > maxPosition)
    {
        return false;
    }

    return true;
}

void RiskEngine::activateKillSwitch()
{
    killSwitchActive = true;
}

void RiskEngine::deactivateKillSwitch()
{
    killSwitchActive = false;
}

bool RiskEngine::isKillSwitchActive() const
{
    return killSwitchActive;
}