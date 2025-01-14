#include "Player.hpp"
#include <stdexcept>
#include <numeric> // For accumulate if desired

Player::Player(const std::string &name, float initialMoney)
    : name(name), money(initialMoney), currentBet(0.f)
{
}

void Player::addCard(std::shared_ptr<Card> card)
{
    hand.push_back(card);
}

void Player::clearHand()
{
    hand.clear();
    currentBet = 0.f;
}

int Player::calculateHandTotal() const
{
    int total = 0;
    int numAces = 0;

    // First count aces and sum up non-ace cards
    for (const auto &card : hand)
    {
        int val = card->getValue();

        if (val == 1)
        { // Ace
            numAces++;
        }
        else if (val > 10 && val < 14)
        { // Face cards
            total += 10;
        }
        else
        {
            total += val;
        }
    }

    // Now handle aces optimally
    for (int i = 0; i < numAces; i++)
    {
        // If we can add 11 without busting, do it. Otherwise add 1
        if (total + 11 <= 21)
        {
            total += 11;
        }
        else
        {
            total += 1;
        }
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
    return (amount <= money);
}

void Player::setCurrentBet(float newBet)
{
    currentBet = newBet;
}

const std::string &Player::getName() const
{
    return name;
}

bool Player::isBusted() const
{
    return (calculateHandTotal() > 21);
}

void Player::addWildcard(std::shared_ptr<Wildcard> card)
{
    if (card)
    {
        wildcards.push_back(card);
    }
}

bool Player::useWildcard(size_t index, std::vector<Player> &allPlayers)
{
    if (index >= wildcards.size())
    {
        return false;
    }

    wildcards[index]->use(*this, allPlayers);
    // Remove the wildcard once used
    wildcards.erase(wildcards.begin() + index);
    return true;
}

const std::vector<std::shared_ptr<Wildcard>> &Player::getWildcards() const
{
    return wildcards;
}

bool Player::hasWildcards() const
{
    return !wildcards.empty();
}

void Player::clearWildcards()
{
    wildcards.clear();
}

void Player::doubleBet(float amount)
{
    if (canBet(amount))
    {
        money -= amount;
    }
}

void Player::setTotal()
{
    hand.clear();

    auto card1 = std::make_shared<Card>(10, Suit::Hearts);
    auto card2 = std::make_shared<Card>(10, Suit::Diamonds);

    hand.push_back(card1);
    hand.push_back(card2);
}

bool Player::hasForesight()
{
    if (foresight == true)
    {
        return true;
    }

    else
    {
        return false;
    }
}

void Player::setForesight()
{
    foresight = true;
}

void Player::setForesightFalse()
{
    foresight = false;
}
// SafeHit wildcard
bool Player::wasSafeHitApplied() const {
    return std::any_of(wildcards.begin(), wildcards.end(), 
                       [](const auto& w) { return w->getName() == "Safe Hit"; });
}

void Player::hit(Deck& deck) {
    auto card = deck.draw();
    hand.push_back(std::make_shared<Card>(card));

    if (calculateHandTotal() > 21) {
        for (auto it = wildcards.begin(); it != wildcards.end(); ++it) {
            if ((*it)->getName() == "Safe Hit") {
                hand.pop_back();
                wildcards.erase(it);
                return;
            }
        }
    }
}