#include "Player.hpp"

Player::Player(const std::string &name, float initialMoney)
    : name(name), money(initialMoney), currentBet(0.0f)
{
}

void Player::addCard(std::shared_ptr<Card> card)
{
    hand.push_back(card);
}

void Player::clearHand()
{
    hand.clear();
    currentBet = 0.0f;
    hasOveriddenHandTotal = false;
}

int Player::calculateHandTotal() const
{
    if (hasOveriddenHandTotal)
    {
        return overiddenHandTotal;
    }

    int total = 0;
    for (const auto &card : hand)
    {
        total += card->getValue();
    }
    return total;
}

const std::vector<std::shared_ptr<Card>> &Player::getHand() const
{
    return hand;
}

void Player::placeBet(float amount)
{
    if (canBet(amount))
    {
        money -= amount;
        currentBet += amount;
    }
}

void Player::addWinnings(float amount)
{
    money += amount;
}

float Player::getMoney() const
{
    return money;
}

float Player::getCurrentBet() const
{
    return currentBet;
}

bool Player::canBet(float amount) const
{
    return amount <= money;
}

const std::string &Player::getName() const
{
    return name;
}

bool Player::isBusted() const
{
    return calculateHandTotal() > 21;
}

void Player::addWildcard(std::shared_ptr<Wildcard> wildcard)
{
    wildcards.push_back(wildcard);
}

void Player::clearWildcards()
{
    wildcards.clear();
}

const std::vector<std::shared_ptr<Wildcard>> &Player::getWildcards() const
{
    return wildcards;
}

void Player::useWildcard(int index, std::vector<Player> &allPlayers)
{
    if (index >= 0 && index < wildcards.size())
    {
        wildcards[index]->use(*this, allPlayers);
        wildcards.erase(wildcards.begin() + index);
    }
}

void Player::setHandTotal(int total)
{
    hasOveriddenHandTotal = true;
    overiddenHandTotal = total;
}