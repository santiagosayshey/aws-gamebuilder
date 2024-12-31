#pragma once
#include "State.hpp"
#include "../ui/Button.hpp"
#include <vector>
#include <memory>
#include <iostream>

class Card {
public:
    Card(int value);
    void setPosition(const sf::Vector2f& pos);
    void draw(sf::RenderWindow& window, const sf::Font& font, bool faceUp = true);
    int getValue() const { return value; }

private:
    sf::RectangleShape shape;
    sf::Text text;
    int value;
};

class GameState : public State {
public:
    GameState(sf::RenderWindow& window);
    
    void handleInput() override;
    void update() override;
    void render() override;

private:
    void initializeButtons();
    void loadResources();
    void dealInitialCards();
    void dealerPlay();
    void checkGameOver();
    int calculateHandTotal(const std::vector<std::shared_ptr<Card>>& hand);
    sf::Vector2f getMousePosition() const;
    void resetGame();
    void startNewBet();
    void handleWin();
    void handleLoss();
    void handlePush();
    void updateMoneyText();

    sf::Font font;
    std::vector<Button> buttons;
    
    std::vector<std::shared_ptr<Card>> playerHand;
    std::vector<std::shared_ptr<Card>> dealerHand;
    
    bool playerTurn;
    bool gameOver;
    bool dealerRevealed;
    bool bettingPhase;
    
    float playerMoney;
    float currentBet;
    float minBet;
    
    sf::Text playerScoreText;
    sf::Text dealerScoreText;
    sf::Text messageText;
    sf::Text moneyText;
    sf::Text betText;
};