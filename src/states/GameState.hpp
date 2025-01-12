#pragma once

#include "State.hpp"
#include "GameSettingsState.hpp"
#include "../core/Deck.hpp"
#include "../core/WildcardDeck.hpp"
#include "../entity/Player.hpp"
#include "../ui/Button.hpp"
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>

class GameState : public State {
public:
    GameState(sf::RenderWindow& window, const GameSettings& settings);

    void handleInput() override;
    void update() override;
    void render() override;

private:
    // Round flow
    void startNewRound();
    void dealInitialCards();
    void dealWildcards();
    void nextPlayer();
    void concludeRound();

    // UI updates
    void updateLabels();

    // Data
    GameSettings settings;
    Deck deck;
    WildcardDeck wildcardDeck;
    std::vector<Player> players;
    int currentPlayerIndex;
    bool roundInProgress;
    bool roundConcluded;

    // Buttons (default-constructed in the initializer list)
    Button hitButton;
    Button standButton;
    Button wildcardButton;

    // Font & Text
    sf::Font font;
    sf::Text roundInfoText;    
    sf::Text wildcardInfoText; 
    sf::Text messageText;      
};
