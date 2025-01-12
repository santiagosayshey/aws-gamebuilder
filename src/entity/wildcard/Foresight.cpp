#include "Foresight.hpp"
#include "../Player.hpp"
#include "../../states/GameState.hpp"  // If needed to access the deck
#include <iostream>

void Foresight::use(Player& owner, std::vector<Player>& allPlayers) {
    (void)allPlayers;
    // In a real implementation, you'd peek the next card in the deck
    // E.g., if your GameState has a global or accessible pointer to the deck. 
    // For now, we just do a debug log:
    std::cout << owner.getName() << " used Foresight to see the next card (not implemented)!\n";
}

std::string Foresight::getName() const {
    return "Foresight";
}

std::string Foresight::getDescription() const {
    return "Allows you to see the next card in the deck.";
}

Wildcard::Timing Foresight::getUsageTiming() const {
    return Timing::ANY_TIME;
}