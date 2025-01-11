#include "Player.hpp"

#include <stdexcept>

Player::Player(const std::string &name, float initialMoney)
    : name(name), money(initialMoney), currentBet(0.0f) {}

void Player::addCard(std::shared_ptr<Card> card) { hand.push_back(card); }

void Player::clearHand() {
  hand.clear();
  currentBet = 0.0f;
}

int Player::calculateHandTotal() const {
  int total = 0;
  int aceCount = 0;

  for (const auto &card : hand) {
    if (card->isAce()) {
      ++aceCount;
      total += 11;
    } else {
      total += card->getValue();
    }
  }

  // Adjust Aces if the total exceeds 21
  while (total > 21 && aceCount > 0) {
    total -= 10;
    --aceCount;
  }

  return total;
}

const std::vector<std::shared_ptr<Card>> &Player::getHand() const {
  return hand;
}

void Player::placeBet(float amount) {
  if (canBet(amount)) {
    money -= amount;
    currentBet += amount;
  }
}

void Player::addWinnings(float amount) { money += amount; }

float Player::getMoney() const { return money; }

float Player::getCurrentBet() const { return currentBet; }

bool Player::canBet(float amount) const { return amount <= money; }

const std::string &Player::getName() const { return name; }

bool Player::isBusted() const { return calculateHandTotal() > 21; }

void Player::addWildcard(std::shared_ptr<Wildcard> card) {
  if (card) {
    wildcards.push_back(card);
  }
}

bool Player::useWildcard(size_t index, std::vector<Player> &allPlayers) {
  if (index >= wildcards.size()) {
    return false;
  }

  wildcards[index]->use(*this, allPlayers);
  wildcards.erase(wildcards.begin() + index);
  return true;
}

const std::vector<std::shared_ptr<Wildcard>> &Player::getWildcards() const {
  return wildcards;
}

bool Player::hasWildcards() const { return !wildcards.empty(); }

void Player::clearWildcards() { wildcards.clear(); }

void Player::doubleBet() {
  if (canBet(currentBet)) {
    money -= currentBet;
    currentBet *= 2;
  }
}

void Player::setTotal() {
  hand.clear();

  auto card1 = std::make_shared<Card>(10, Suit::HEARTS);
  auto card2 = std::make_shared<Card>(10, Suit::DIAMONDS);

  hand.push_back(card1);
  hand.push_back(card2);
}