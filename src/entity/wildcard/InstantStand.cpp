#include "InstantStand.hpp"
#include "../Player.hpp"
#include <algorithm>

void InstantStand::use(Player& owner, std::vector<Player>& allPlayers) {
    (void)allPlayers;
    // For demonstration, set the player's total to 20 
    // (though a real implementation might do something else)
    // A hacky approach: remove all cards and add a dummy "20" card, or track a 'forced total.'
    // For now, we just pretend we can do it magically:
    // Clear hand, push a single card with value=20. (Not realistic, but just an example)
    owner.clearHand();
    // push a new "Card(20)" to mimic forced total
    owner.addCard(std::make_shared<Card>(20));
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