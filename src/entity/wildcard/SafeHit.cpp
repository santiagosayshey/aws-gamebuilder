
#include "SafeHit.hpp"
#include "entity/Player.hpp"

void SafeHit::use(Player &owner, std::vector<Player> &allPlayers) {}

std::string SafeHit::getName() const
{
    return "Safe Hit";
}

std::string SafeHit::getDescription() const
{
    return "Allows one hit where you won't bust; if the hit would take you over 21, your total returns to what it was pre-hit";
}

Wildcard::Timing SafeHit::getUsageTiming() const
{
    return Wildcard::Timing::AFTER_DRAW;
}
