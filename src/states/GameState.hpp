#pragma once
#include "State.hpp"
#include "../ui/Button.hpp"
#include "../entity/Player.hpp"
#include "../entity/Card.hpp"
#include "../entity/Deck.hpp"
#include "GameSettingsState.hpp"
#include <vector>
#include <memory>
#include <iostream>

class GameState : public State
{
public:
    GameState(sf::RenderWindow &window, const GameSettings &settings = GameSettings());

    void handleInput() override;
    void update() override;
    void render() override;

private:
    void initializeButtons();
    bool loadResources();
    void dealInitialCards();
    void dealerPlay();
    void checkGameOver();
    sf::Vector2f getMousePosition() const;
    void resetGame();
    void startNewBet();
    void handleWin(Player &player);
    void handleLoss(Player &player);
    void handlePush(Player &player);
    void updateMoneyText();
    void nextPlayer(); // Move to next player's turn
    size_t getCurrentPlayerIndex() const { return currentPlayerIndex; }

    sf::Font font;
    std::vector<Button> buttons;

    std::vector<Player> players; // Vector of all players
    size_t currentPlayerIndex;   // Current player's turn
    Player dealer;
    Deck deck;

    bool playerTurn;       // True if it's players' turns, false if dealer's turn
    bool gameOver;         // True if round is over
    bool dealerRevealed;   // True if dealer's cards are revealed
    bool bettingPhase;     // True if in betting phase
    bool canDoubleDown;    // True if player can double
    bool wildcardsEnabled; // True if wildcards are enabled

    float minBet;

    // Text elements for each player
    std::vector<sf::Text> playerScoreTexts;
    std::vector<sf::Text> playerMoneyTexts;
    std::vector<sf::Text> playerBetTexts;
    sf::Text dealerScoreText;
    sf::Text messageText;
    sf::Text currentPlayerText; // Shows which player's turn it is
};