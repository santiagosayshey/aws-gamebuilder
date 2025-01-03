#include "Wildcards.hpp"
#include "entity/Player.hpp"

class ExtraBet : public Wildcard {
public:
    virtual ~Tycoon() {}

    void use(Player& owner, std::vector<Player>& players) override {
        // Get current bet
        float currentBet = owner.getCurrentBet();
        
        // Double the player's bet
        owner.setCurrentBet(currentBet * 2);
    }
};



