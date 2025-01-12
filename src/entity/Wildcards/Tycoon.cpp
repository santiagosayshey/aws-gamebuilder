#include "Wildcards.hpp"
#include "entity/Player.hpp"

class Tycoon : public Wildcard {
public:
    virtual ~Tycoon() {}

    void use(Player& owner, std::vector<Player>& players) override {
        (void)players;
        // Get current money
        float currentMoney = owner.getMoney();
        
        // Double the money by adding the current amount as winnings
        owner.addWinnings(currentMoney);
    }

    std::string getName() const override {
        return "Tycoon";
    }

    std::string getDescription() const override {
        return "Doubles your winnings";
    }

    Timing getUsageTiming() const override {
        return Timing::ANY_TIME;
    }
};



