#include "Wildcards.hpp"
#include "entity/Player.hpp"

class ExtraBet : public Wildcard {
public:
    virtual ~ExtraBet() {}

    void use(Player& owner, std::vector<Player>& players) override {
        // Get current money
        float currentMoney = owner.getMoney();
        
        // Double the money by adding the current amount as winnings
        owner.addWinnings(currentMoney);
    }
};



