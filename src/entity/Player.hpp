#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Card.hpp"
#include "Wildcards.hpp"

class Player
{
public:
    Player(const std::string &name, float initialMoney);

    // Hand management
    void addCard(std::shared_ptr<Card> card);
    void clearHand();
    int calculateHandTotal() const;
    void setHandTotal(int total);
    const std::vector<std::shared_ptr<Card>> &getHand() const;

    // Money management
    void placeBet(float amount);
    void addWinnings(float amount);
    float getMoney() const;
    float getCurrentBet() const;
    bool canBet(float amount) const;

    // Wildcard management
    void addWildcard(std::shared_ptr<Wildcard> wildcard);
    void clearWildcards();
    const std::vector<std::shared_ptr<Wildcard>> &getWildcards() const;
    void useWildcard(int index, std::vector<Player> &allPlayers);

    // Getters
    const std::string &getName() const;
    bool isBusted() const;

private:
    std::string name;
    std::vector<std::shared_ptr<Card>> hand;
    std::vector<std::shared_ptr<Wildcard>> wildcards;
    float money;
    float currentBet;
    bool hasOveriddenHandTotal = false;
    int overiddenHandTotal = 0;
};