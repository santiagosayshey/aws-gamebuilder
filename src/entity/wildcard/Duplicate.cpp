#include "Duplicate.hpp"
#include "entity/Player.hpp"
#include <random>

class Duplicate : public Wildcard
{
public:
    
    Duplicate::Duplicate() {}

    Duplicate::~Duplicate() {}

    void Duplicate::use(Player& owner, std::vector<Player>& players) override {
        std::vector<std::shared_ptr<Wildcard>> availableWildcards;

        // Collect all valid wildcards from other players
        for (auto& player : players) {
            if (&player != &owner) {  // Exclude the owner
                for (auto &wc : player.getWildcards()) {
                    if (wc->getName() != "Duplicate") {  // Exclude "Duplicate" wildcards
                        availableWildcards.push_back(wc);
                    }
                }
            }
        }
        if (!availableWildcards.empty()) {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dist(0, availableWildcards.size() - 1);
            int randomIndex = dist(gen);
            std::shared_ptr<Wildcard> selectedWildcard = availableWildcards[randomIndex];

            // Add the selected wildcard to the owner's list
            owner.addWildcard(selectedWildcard);
        }
    }

    std::string Duplicate::getName() const override {
        return "Duplicate";
    }

    std::string Duplicate::getDescription() const override {
        return "Allows you to duplicate a random wildcard from another player.";
    }

    Timing Duplicate::getUsageTiming() const override {
        return Timing:: ANY_TIME;
    }
};