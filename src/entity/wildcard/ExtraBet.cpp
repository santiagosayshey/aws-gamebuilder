#include "ExtraBet.hpp"
#include "entity/Player.hpp"

class ExtraBet : public Wildcard {
public:
    ExtraBet::ExtraBet() {}

    ExtraBet::~ExtraBet() {}

    void ExtraBet::use(Player& owner, std::vector<Player>& players) override {
        (void)players;
        // Get current bet
        float currentBet = owner.getCurrentBet();
        
        // Double the player's bet
        owner.setCurrentBet(currentBet * 2);
    }

    std::string ExtraBet::getName() const override {
        return "Extra Bet";
    }

    std::string ExtraBet::getDescription() const override {
        return "Doubles the player's current bet.";
    }

    Timing ExtraBet::getUsageTiming() const override {
        return Timing::AFTER_DRAW;
    }
};



