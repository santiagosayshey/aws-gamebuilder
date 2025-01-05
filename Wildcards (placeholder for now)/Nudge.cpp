#pragma once
#include "Wildcards.hpp"
#include "entity/Player.hpp"

// incomplete
class Nudge : public Wildcard
{
public:
    Nudge() = default;
    ~Nudge() = default;

    void use(Player &owner, std::vector<Player> &allPlayers) override
    {
        int adjustment = getPlayerNudgeChoice(1);
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

private:
    int getCardValue() const
    {
        // Logic to get the card value
        return 0;
    }

    int getPlayerNudgeChoice(int currentCardValue) const
    {
        // Get the player's choice
        int choice;
        // Logic to get the player's choice

        // Return the new card value
        return choice == 1 ? currentCardValue + 1 : currentCardValue - 1;
    }
};
