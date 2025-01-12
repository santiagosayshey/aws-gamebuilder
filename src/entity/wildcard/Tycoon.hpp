#pragma once
#include "Wildcard.hpp"

class Tycoon : public Wildcard {
public:
    ~Tycoon() override = default;
    void use(Player& owner, std::vector<Player>& allPlayers) override;
    std::string getName() const override;
    std::string getDescription() const override;
    Timing getUsageTiming() const override;
};