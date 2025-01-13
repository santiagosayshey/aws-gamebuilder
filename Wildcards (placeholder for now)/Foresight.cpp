#pragma once
#include "Wildcards.hpp"
#include "entity/Player.hpp"
#include "entity/Card.hpp"
#include <vector>
#include <iostream>

// incomplete
class Foresight : public Wildcard
{
public:
    Foresight() = default;
    ~Foresight() = default;

    void use(Player &owner, std::vector<Player> &allPlayers) override
    {
        // logic
    }

    std::string getName() const override
    {
        return "Foresight";
    }

    std::string getDescription() const override
    {
        return "See the next card that will be drawn.";
    }

    Timing getUsageTiming() const override
    {
        return Timing::BEFORE_DRAW;
    }
};