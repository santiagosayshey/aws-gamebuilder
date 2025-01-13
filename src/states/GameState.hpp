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
    void startNewRound();
    void dealInitialCards();
    void dealWildcards();
    void nextPlayer();
    void concludeRound();
    void updateLabels();

    GameSettings settings;
    Deck deck;
    WildcardDeck wildcardDeck;
    std::vector<Player> players;

    int currentPlayerIndex;
    bool roundInProgress;
    bool roundConcluded;

    bool waitingForReplay;

    // Buttons
    Button hitButton;
    Button standButton;
    Button wildcardButton;

    // Font & text
    sf::Font font;
    sf::Text roundInfoText;
    sf::Text wildcardInfoText;
    sf::Text messageText;
};
