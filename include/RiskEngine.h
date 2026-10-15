#pragma once
#include <string>
#include <cstdint>
#include "Order.h"

class RiskEngine
{
    private:
        std::int64_t maxPosition;
        std::uint64_t maxOrderQuantity;
        double maxNotional;
        bool killSwitchActive;
    public:
        RiskEngine(std::int64_t maxPosition, std::uint64_t maxOrderQuantity, double maxNotional);
        bool validateOrder(const Order& order, std::int64_t currentPosition) const;
        void activateKillSwitch();
        void deactivateKillSwitch();
        bool isKillSwitchActive() const;

};