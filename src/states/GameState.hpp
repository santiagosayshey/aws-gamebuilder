#pragma once

#include "State.hpp"
#include "GameSettingsState.hpp"
#include "BettingState.hpp"
#include "../core/Deck.hpp"
#include "../core/WildcardDeck.hpp"
#include "../entity/Player.hpp"
#include "../ui/Button.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

class GameState : public State {
public:
    GameState(sf::RenderWindow& window, const GameSettings& settings, const std::vector<PlayerBet>& initialBets);
    
    void handleInput() override;
    void update() override;
    void render() override;
    const Card& peekNextCard() const;

private:
    void startNewRound();
    void dealInitialCards();
    void dealWildcards();
    void nextPlayer();
    void concludeRound();
    void updateLabels();
    void initializeDecorations(); 

    // Game settings and state
    GameSettings settings;
    std::vector<Player> players;
    std::vector<PlayerBet> initialBets; 
    int currentPlayerIndex;
    bool roundInProgress;
    bool roundConcluded;
    bool waitingForReplay;
    bool isWildcardHovered;

    // Game systems
    Deck deck;
    WildcardDeck wildcardDeck;

    // Game buttons
    Button hitButton;
    Button standButton;
    Button doubleDownButton;
    Button wildcardButton;

    // Font & texts
    sf::Font font;
    sf::Text roundInfoText;
    sf::Text wildcardInfoText;
    sf::Text messageText;
    sf::Text playerMoneyText; 
    sf::Text wildcardHoverText;

    // Utility variables
    float animationTime = 0.0f;
    
    // Decorative elements
    struct CircleData {
        sf::CircleShape shape;
        sf::Vector2f basePos;
        float xFreq;
        float yFreq;
        float xAmp;
        float yAmp;
        float phase;
    };
    std::vector<CircleData> decorativeCircles;
};