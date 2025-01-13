#include "ExtraBet.hpp"
#include "entity/Player.hpp" 

ExtraBet::ExtraBet() {}

ExtraBet::~ExtraBet() {}

void ExtraBet::use(Player& owner, std::vector<Player>& players) {
    (void)players; 
    // Get current bet
    float currentBet = owner.getCurrentBet();
    
    // Double the player's bet
    owner.setCurrentBet(currentBet * 2);
}

std::string ExtraBet::getName() const {
    return "Extra Bet";
}

std::string ExtraBet::getDescription() const {
    return "Doubles the player's current bet.";
}

ExtraBet::Timing ExtraBet::getUsageTiming() const {
    return Timing::AFTER_DRAW;
}
