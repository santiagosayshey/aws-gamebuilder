
#include "Foresight.hpp"
#include "entity/Player.hpp"
#include "states/GameState.hpp"

void Foresight::use(Player &owner, std::vector<Player> &)
{
    owner.setForesight();
}

std::string Foresight::getName() const
{
    return "Foresight";
}

std::string Foresight::getDescription() const
{
    return "Allows you to see the next card in the deck when pressing hit.";
}

Wildcard::Timing Foresight::getUsageTiming() const
{
    return Wildcard::Timing::AFTER_DRAW;
}
