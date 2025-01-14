#pragma once
#include "Wildcard.hpp"
#include "states/GameState.hpp"

class Foresight : public Wildcard
{
public:
    ~Foresight() override = default;
    void use(Player &owner, std::vector<Player> &allPlayers) override;
    std::string getName() const override;
    std::string getDescription() const override;
    Timing getUsageTiming() const override;
};