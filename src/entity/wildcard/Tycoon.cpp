#include "Tycoon.hpp"
#include "../Player.hpp"

void Tycoon::use(Player& owner, std::vector<Player>&) {
    float currentMoney = owner.getMoney();
    // Double it
    owner.addWinnings(currentMoney);
}

std::string Tycoon::getName() const {
    return "Tycoon";
}

std::string Tycoon::getDescription() const {
    return "Doubles your current money instantly.";
}

Wildcard::Timing Tycoon::getUsageTiming() const {
    return Timing::ANY_TIME;
}