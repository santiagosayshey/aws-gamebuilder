#include "Tycoon.hpp"
#include "entity/Player.hpp"

Tycoon::Tycoon() {}

Tycoon::~Tycoon() {}

void Tycoon::use(Player& owner, std::vector<Player>& players) {
    (void)players; 
    // Get current money
    float currentMoney = owner.getMoney();
    
    // Double the money by adding the current amount as winnings
    owner.addWinnings(currentMoney);
}

std::string Tycoon::getName() const {
    return "Tycoon";
}

std::string Tycoon::getDescription() const {
    return "Doubles your current money.";
}

Tycoon::Timing Tycoon::getUsageTiming() const {
    return Timing::ANY_TIME;
}
