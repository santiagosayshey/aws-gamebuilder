
#include "InstantStand.hpp"
#include "entity/Player.hpp"

void InstantStand::use(Player &owner, std::vector<Player> &allPlayers)
{
    owner.setTotal();
}

std::string InstantStand::getName() const
{
    return "Instant Stand";
}

std::string InstantStand::getDescription() const
{
    return "Sets your hand total to 20.";
}

Wildcard::Timing InstantStand::getUsageTiming() const
{
    return Wildcard::Timing::AFTER_DRAW;
}
