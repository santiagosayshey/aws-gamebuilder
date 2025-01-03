#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Card.hpp"

class Player {
public:
    Player(const std::string& name, float initialMoney);
    
    // Hand management
    void addCard(std::shared_ptr<Card> card);
    void clearHand();
    int calculateHandTotal() const;
    const std::vector<std::shared_ptr<Card>>& getHand() const;
    void addWildcard(std::shared_ptr<Wildcard> wildcard);
    void useWildcard(size_t index, std::vector<std::shared_ptr<Player>>& players, Deck& deck);
    const std::vector<std::shared_ptr<Wildcard>>& getWildcards() const;
    
    // Money management
    void placeBet(float amount);
    void addWinnings(float amount);
    float getMoney() const;
    float getCurrentBet() const;
    bool canBet(float amount) const;
    void setCurrentBet(float newBet); // new function for the wildcard "ExtraBet"
    
    // Getters
    const std::string& getName() const;
    bool isBusted() const;
    
private:
    std::string name;
    std::vector<std::shared_ptr<Card>> hand;
    std::vector<std::shared_ptr<Wildcard>> wildcards;
    float money;
    float currentBet;
};