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
                int total = firstCard -> getValue() + 
                secondCard -> getValue();
                int average = total / 2;

                // set both card into average (ensuring its not a float value)
                firstCard -> setValue(std::floor(average));
                secondCard -> setValue(std::ceil(average));
            }
        }

        std::string getName() const override {
            return "Balance Shift";
        }

        std::string getDescription() const override {
            return "Balance the value of the cards between 2 player";
        }

        Timing getUsageTiming() const override {
            return Timing::ANY_TIME;
        }
};
