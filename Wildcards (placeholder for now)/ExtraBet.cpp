#include "Wildcards.hpp"


class ExtraBet : public Wildcard {
public:
// need a player class that has current money function
    void activate(Player& player, std::vector<std::shared_ptr<Player>>& players, Deck& deck) override {
        //double currentMoney = player.getCurrentMoney();
        //player.setCurrentMoney(currentMoney * 2);
        //std::cout << "Extra Bet activated: Player's money doubled to " << player.getCurrentMoney() << std::endl;
    }

    std::string description() const override {
        return "Player's money doubled";
    }
};

// make sure Player class has 
// the getCurrentMoney() and setCurrentMoney()

