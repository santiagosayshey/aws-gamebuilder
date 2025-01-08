#pragma once
#include "Wildcards.hpp"
#include "entity/Player.hpp"

class Peacemaker : public Wildcard
{
public:
    Peacemaker() = default;
    ~Peacemaker() = default;

    void use(Player &owner, GameState &gameState) override
    {
        gameState.wildcardsEnabled = false;
    }

    std::string getName() const override
    {
        return "Peacemaker";
    }

    std::string getDescription() const override
    {
        return "For one round, wildcards are disabled.";
    }

    Timing getUsageTiming() const override
    {
        return Timing::ANY_TIME;
    }
};