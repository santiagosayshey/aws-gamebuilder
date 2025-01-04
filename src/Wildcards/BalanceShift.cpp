#include "Wildcards.hpp"
#include "entity/Player.hpp"
#include <algorithm> 

class BalanceShift: public Wildcard {
public:
        virtual ~BalanceShift() {}

        void use(Player& owner, std::vector<Player>& targets) override {
            auto& hand = owner.getHand();

            if (hand.size() >= 2) {
                // select two random cards
                std:: random_device rd;
                std:: mt19937 gen(rd());
                std:: shuffle(hand.begin(), hand.end(), gen);

                auto& firstCard = hand[0];
                auto& secondCard = hand[1];
                int average = (firstCard -> getValue() + 
                secondCard -> getValue()) / 2;

                // set both card into average (make sure it's not a float value)
                firstCard -> setValue(std::floor(average));
                secondCard -> setValue(std::ceil(average));
            }
        }
};
