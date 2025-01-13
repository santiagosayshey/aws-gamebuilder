#include "Player.hpp"
#include <stdexcept>
#include <numeric> // For accumulate if desired

Player::Player(const std::string& name, float initialMoney)
    : name(name)
    , money(initialMoney)
    , currentBet(0.f)
{
}

void Player::addCard(std::shared_ptr<Card> card) {
    hand.push_back(card);
}

void Player::clearHand() {
    hand.clear();
    currentBet = 0.f;
}

int Player::calculateHandTotal() const {
    int total = 0;
    for (auto& c : hand) {
        int val = c->getValue(); 
        if (val > 10 && val < 14) {
            val = 10; // face cards become 10
        }
        total += val;
    }
    return total;
}


const std::vector<std::shared_ptr<Card>>& Player::getHand() const {
    return hand;
}

void Player::placeBet(float amount) {
    if (canBet(amount)) {
        money -= amount;
        currentBet += amount;
    }
}

void Player::addWinnings(float amount) {
    money += amount;
}

float Player::getMoney() const {
    return money;
}

float Player::getCurrentBet() const {
    return currentBet;
}

bool Player::canBet(float amount) const {
    return (amount <= money);
}

void Player::setCurrentBet(float newBet) {
    currentBet = newBet;
}

const std::string& Player::getName() const {
    return name;
}

bool Player::isBusted() const {
    return (calculateHandTotal() > 21);
}

void Player::addWildcard(std::shared_ptr<Wildcard> card) {
    if (card) {
        wildcards.push_back(card);
    }
}

bool Player::useWildcard(size_t index, std::vector<Player>& allPlayers) {
    if (index >= wildcards.size()) {
        return false;
    }
    wildcards[index]->use(*this, allPlayers);
    // Remove the wildcard once used
    wildcards.erase(wildcards.begin() + index);
    return true;
}

const std::vector<std::shared_ptr<Wildcard>>& Player::getWildcards() const {
    return wildcards;
}

bool Player::hasWildcards() const {
    return !wildcards.empty();
}

void Player::clearWildcards() {
    wildcards.clear();
}
