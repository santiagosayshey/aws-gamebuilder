#pragma once
#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include "Card.hpp"
#include "../core/Deck.hpp"
#include "wildcard/Wildcard.hpp"

class Player
{
public:
    Player(const std::string &name, float initialMoney);

    // Hand management
    void addCard(std::shared_ptr<Card> card);
    void clearHand();
    int calculateHandTotal() const;
    const std::vector<std::shared_ptr<Card>> &getHand() const;
    void setTotal();

    void placeBet(float amount);
    void doubleBet(float amount);
    void addWinnings(float amount);
    float getMoney() const;
    float getCurrentBet() const;
    bool canBet(float amount) const;

    void setCurrentBet(float newBet); // For wildcards that alter bet

    const std::string &getName() const;
    bool isBusted() const;

    // Wildcard management
    void addWildcard(std::shared_ptr<Wildcard> card);
    bool useWildcard(size_t index, std::vector<Player> &allPlayers);
    const std::vector<std::shared_ptr<Wildcard>> &getWildcards() const;
    bool hasWildcards() const;
    void clearWildcards();
    bool hasForesight();
    void setForesight();
    void setForesightFalse();

    // SafeHit wildcard functions
    void hit(Deck& deck);
    bool wasSafeHitApplied() const;
    
private:
    std::string name;
    std::vector<std::shared_ptr<Card>> hand;
    std::vector<std::shared_ptr<Wildcard>> wildcards;
    float money;
    float currentBet;
    bool foresight = false;
    bool safeHitUsed = false;
};
