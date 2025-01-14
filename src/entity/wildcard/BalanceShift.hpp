#pragma once
#include "Wildcard.hpp"
#include <numeric>

class BalanceShift : public Wildcard {
public:
    ~BalanceShift() override = default;
    void use(Player& owner, std::vector<Player>& allPlayers) override;
    std::string getName() const override;
    std::string getDescription() const override;
    Timing getUsageTiming() const override;
};