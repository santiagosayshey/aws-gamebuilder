#include "SafeHit.hpp"
#include "entity/Player.hpp"

SafeHit::SafeHit() {}

SafeHit::~SafeHit() {}

void SafeHit::use(Player& owner, std::vector<Player>& allPlayers) {
    owner.activateSafeHit();
}

std::string SafeHit::getName() const {
    return "Safe Hit";
}

std::string SafeHit::getDescription() const {
    return "Allows one hit without busting. If the hit would bust you, revert to the previous total.";
}

Wildcard::Timing SafeHit::getUsageTiming() const {
    return Timing::BEFORE_DRAW; // Specifies when the wildcard can be used.
}
