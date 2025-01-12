#include "Wildcards.hpp"
#include "entity/Player.hpp"

class ExtraBet : public Wildcard {
public:
    virtual ~ExtraBet() {}

    void use(Player& owner, std::vector<Player>& players) override {
        (void)players;
        // Get current bet
        float currentBet = owner.getCurrentBet();
        
        // Double the player's bet
        owner.setCurrentBet(currentBet * 2);
    }

    std::string getName() const override {
        return "Extra Bet";
    }

    std::string getDescription() const override {
        return "Doubles the player's current bet.";
    }

    Timing getUsageTiming() const override {
        return Timing::AFTER_DRAW;
    }
};



