#include "Tycoon.hpp"
#include "entity/Player.hpp"

class Tycoon : public Wildcard {
public:
    Tycoon::Tycoon() {}

    Tycoon::~Tycoon() {}

    void Tycoon::use(Player& owner, std::vector<Player>& players) override {
        (void)players;
        // Get current money
        float currentMoney = owner.getMoney();
        
        // Double the money by adding the current amount as winnings
        owner.addWinnings(currentMoney);
    }

    std::string Tycoon::getName() const override {
        return "Tycoon";
    }

    std::string Tycoon::getDescription() const override {
        return "Doubles your winnings";
    }

    Timing Tycoon::getUsageTiming() const override {
        return Timing::ANY_TIME;
    }
};



