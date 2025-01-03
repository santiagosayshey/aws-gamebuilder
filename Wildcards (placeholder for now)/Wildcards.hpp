#pragma once
#include <string>
#include <memory>
#include <vector>

// Forward declarations
class Player;

class Wildcard
{
public:
    virtual ~Wildcard() = default;

    // Execute the wildcard's effect
    virtual void use(Player &owner, std::vector<Player> &allPlayers) = 0;

    // Get the wildcard's name
    virtual std::string getName() const = 0;

    // Get the wildcard's description
    virtual std::string getDescription() const = 0;

    // When can this wildcard be used?
    enum class Timing
    {
        BEFORE_DRAW, // Before drawing a card
        AFTER_DRAW,  // After drawing a card
        ANY_TIME,    // Any time during the player's turn
        AFTER_BUST   // After busting (going over 21)
    };

    // Get when this wildcard can be used
    virtual Timing getUsageTiming() const = 0;

protected:
    Wildcard() = default;
};