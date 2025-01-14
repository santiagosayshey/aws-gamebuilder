#include "BalanceShift.hpp"
#include "../Player.hpp"

void BalanceShift::use(Player& owner, std::vector<Player>& allPlayers) {
    // Get the player's hand
    auto& ownerHand = owner.getHand();
    if (ownerHand.empty()) {
        return; // No cards to balance
    }

    // Select a random opponent
    std::vector<Player*> opponents;
    for (auto& player : allPlayers) {
        if (&player != &owner) { // Exclude the current player
            opponents.push_back(&player);
        }
    }

    if (opponents.empty()) {
        return; // No opponents available
    }

    // Seed random number generator if not already seeded
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

    // Calculate new balanced value
    int total = std::accumulate(allCardValues.begin(), allCardValues.end(), 0);
    int newValue = total / allCardValues.size(); // Average value

    // Set both players' cards to the new balanced value
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