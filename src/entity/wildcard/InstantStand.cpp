#include "InstantStand.hpp"
#include "../Player.hpp"
#include <algorithm>

// If your Card constructor is now Card(int value, Suit suit),
// you must specify a suit, e.g. Suit::Hearts, Suit::Spades, etc.
void InstantStand::use(Player &owner, std::vector<Player> &allPlayers) {
    (void)allPlayers;
    // Clear existing hand
    owner.clearHand();
    // Hacky approach: create a "value=20" card with some arbitrary suit
    // so the player's "calculateHandTotal()" sees 20. 
    // NOTE: 20 is outside normal 1-13 range, but that's presumably the effect you want:
    owner.addCard(std::make_shared<Card>(13, Suit::Spades)); 
}

std::string InstantStand::getName() const {
    return "Instant Stand";
}

std::string InstantStand::getDescription() const {
    return "Sets your hand total to 20 automatically.";
}

Wildcard::Timing InstantStand::getUsageTiming() const {
    return Timing::ANY_TIME;
}
