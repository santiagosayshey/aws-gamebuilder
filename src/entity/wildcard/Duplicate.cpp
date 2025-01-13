#include "Duplicate.hpp"
#include "../Player.hpp"
#include <random>

void Duplicate::use(Player& owner, std::vector<Player>& allPlayers) {
    // Gather all wildcards from other players
    std::vector<std::shared_ptr<Wildcard>> pool;
    for (auto& p : allPlayers) {
        if (&p == &owner) continue;
        for (auto& wc : p.getWildcards()) {
            if (wc->getName() != "Duplicate") {
                pool.push_back(wc);
            }
        }
    }
    if (!pool.empty()) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<size_t> dist(0, pool.size() - 1);
        size_t idx = dist(gen);
        owner.addWildcard(pool[idx]);
    }
}

std::string Duplicate::getName() const {
    return "Duplicate";
}

std::string Duplicate::getDescription() const {
    return "Duplicates a random wildcard from another player.";
}

Wildcard::Timing Duplicate::getUsageTiming() const {
    return Timing::ANY_TIME;
}