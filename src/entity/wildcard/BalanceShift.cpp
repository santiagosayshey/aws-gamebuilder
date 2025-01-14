#include "BalanceShift.hpp"
#include "../Player.hpp"

void BalanceShift::use(Player& owner, std::vector<Player>& allPlayers) {
    auto& ownerHand = owner.getHand();
    if (ownerHand.empty()) {
        return;
    }

    // Select a random opponent (exclude us from the count)
    std::vector<Player*> opponents;
    for (auto& player : allPlayers) {
        if (&player != &owner && !player.isBusted()) { 
            opponents.push_back(&player);
        }
    }

    if (opponents.empty()) {
        return;
    }

    static bool seeded = false;
    if (!seeded) {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        seeded = true;
    }

    // Select a random opponent
    Player* randomOpponent = opponents[std::rand() % opponents.size()];

    // Collect all card values from both players
    std::vector<int> allCardValues;
    for (const auto& card : ownerHand) {
        allCardValues.push_back(card->getValue());
    }
    
    for (const auto& card : randomOpponent->getHand()) {
        allCardValues.push_back(card->getValue());
    }

    // Calculate new balanced value and get the average 
    int total = std::accumulate(allCardValues.begin(), allCardValues.end(), 0);
    int newValue = total / allCardValues.size(); 

    // Assign new card corresponding to the value
    for (auto& card : ownerHand) {
        card->setValue(newValue);
    }
    
    for (auto& card : randomOpponent->getHand()) {
        card->setValue(newValue);
    }
}


std::string BalanceShift::getName() const {
    return "Balance Shift";
}

std::string BalanceShift::getDescription() const {
    return "Redistributes the values between your card and a random opponent's card.";
}

Wildcard::Timing BalanceShift::getUsageTiming() const {
    return Timing::ANY_TIME;
}