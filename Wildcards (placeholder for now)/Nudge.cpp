#pragma once
#include "Wildcards.hpp"
#include "entity/Player.hpp"

class Nudge : public Wildcard
{
public:
    Nudge() = default;
    ~Nudge() = default;

    void use(Player &owner, std::vector<Player> &allPlayers) override
    {
        owner.activateNudge();
    }

    std::string getName() const override
    {
        return "Nudge";
    }

    std::string getDescription() const override
    {
        return "Increment or decrement your next card by 1.";
    }

    Timing getUsageTiming() const override
    {
        return Timing::ANY_TIME;
    }
};

// Add player class logic for this