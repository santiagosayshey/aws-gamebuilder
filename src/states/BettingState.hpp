#pragma once
#include "State.hpp"
#include "GameSettingsState.hpp"
#include "../ui/Button.hpp"
#include "../entity/Player.hpp"
#include <vector>
#include <string>

struct PlayerBet {
    std::string playerName;
    float startingMoney;
    float currentBet;
    bool hasBet;
};

class BettingState : public State {
public:
    BettingState(sf::RenderWindow& window, const GameSettings& settings);
    
    void handleInput() override;
    void update() override;
    void render() override;

    const std::vector<PlayerBet>& getPlayerBets() const { return playerBets; }
    const GameSettings& getSettings() const { return settings; }

private:
    bool loadResources();
    void initializeButtons();
    void updateLabels();
    void nextPlayer();
    sf::Vector2f getMousePosition() const;
    
    // Game settings passed from previous state
    GameSettings settings;
    
    // Fonts
    sf::Font titleFont;
    sf::Font standardFont;
    sf::Font buttonFont;

    // Button symbols
    std::vector<sf::RectangleShape> minusSymbols;
    std::vector<sf::RectangleShape> plusVerticalSymbols;
    std::vector<sf::RectangleShape> plusHorizontalSymbols;
    
    // Visual elements
    sf::Text titleText;
    sf::Text currentPlayerText;
    sf::Text betAmountText;
    sf::Text instructionText;
    
    // Buttons
    Button increaseBetButton;
    Button decreaseBetButton;
    Button confirmBetButton;
    Button backButton;
    
    // Decorative elements
    struct CircleData {
        sf::CircleShape shape;
        sf::Vector2f basePos;
        float xFreq, yFreq;
        float xAmp, yAmp;
        float phase;
    };
    std::vector<CircleData> decorativeCircles;
    
    // Player betting data
    std::vector<PlayerBet> playerBets;
    size_t currentPlayerIndex;
    float currentBetAmount;
    
    // Constants
    const float BET_INCREMENT = 10.0f;
    const float MIN_BET_MULTIPLIER = 1.0f;  // Minimum bet will be settings.minBet * MIN_BET_MULTIPLIER
    const float MAX_BET_MULTIPLIER = 10.0f; // Maximum bet will be settings.minBet * MAX_BET_MULTIPLIER
    
    void initializeDecorations();
    void validateBetAmount();
};