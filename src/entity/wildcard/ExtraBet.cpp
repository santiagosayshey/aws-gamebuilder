#include "ExtraBet.hpp"
#include "../Player.hpp"

void ExtraBet::use(Player& owner, std::vector<Player>&) {
    float currentBet = owner.getCurrentBet();
    owner.setCurrentBet(currentBet * 2.f);
}

std::string ExtraBet::getName() const {
    return "Extra Bet";
}

std::string ExtraBet::getDescription() const {
    return "Doubles the player's current bet.";
}

Wildcard::Timing ExtraBet::getUsageTiming() const {
    return Timing::AFTER_DRAW;
}