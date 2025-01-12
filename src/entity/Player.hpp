#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Card.hpp"
#include "./entity/wildcard/Wildcard.hpp"

class Player
{
public:
    Player(const std::string &name, float initialMoney);

    // Hand management
    void addCard(std::shared_ptr<Card> card);
    void clearHand();
    int calculateHandTotal() const;
    const std::vector<std::shared_ptr<Card>> &getHand() const;

    // Money management
    void placeBet(float amount);
    void doubleBet();
    void addWinnings(float amount);
    float getMoney() const;
    float getCurrentBet() const;
    void setCurrentBet(float newBet) const;
    bool canBet(float amount) const;

    // Getters
    const std::string &getName() const;
    bool isBusted() const;

    // Wildcard management - structure only, no implementation yet
    void addWildcard(std::shared_ptr<Wildcard> card);
    bool useWildcard(size_t index, std::vector<Player> &allPlayers);
    const std::vector<std::shared_ptr<Wildcard>> &getWildcards() const;
    bool hasWildcards() const;
    void clearWildcards();

private:
    std::string name;
    std::vector<std::shared_ptr<Card>> hand;
    std::vector<std::shared_ptr<Wildcard>> wildcards;
    float money;
    float currentBet;
};