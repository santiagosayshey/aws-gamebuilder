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

    // Game settings and state
    GameSettings settings;
    std::vector<Player> players;
    std::vector<PlayerBet> initialBets;  // Store initial bets
    int currentPlayerIndex;
    bool roundInProgress;
    bool roundConcluded;
    bool waitingForReplay;

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
    sf::Text playerMoneyText;  // Replaces potText

    // Utility variables
    float animationTime = 0.0f;
    
    // Decorative elements
    std::vector<sf::CircleShape> cornerCircles;
    static constexpr float CIRCLE_ANIMATION_SPEED = 0.8f;
};