#include "GameState.hpp"
#include <iostream>     // For logging
#include <cmath>
#include <algorithm>    // for std::max, std::min

GameState::GameState(sf::RenderWindow& window, const GameSettings& settings)
    : State(window)
    , settings(settings)
    , currentPlayerIndex(0)
    , roundInProgress(false)
    , roundConcluded(false)
    // Default-construct the buttons with dummy arguments
    , hitButton(sf::Vector2f(0.f, 0.f), sf::Vector2f(0.f, 0.f), "", sf::Font())
    , standButton(sf::Vector2f(0.f, 0.f), sf::Vector2f(0.f, 0.f), "", sf::Font())
    , wildcardButton(sf::Vector2f(0.f, 0.f), sf::Vector2f(0.f, 0.f), "", sf::Font())
{
    std::cout << "[GameState::CTOR] Begin constructor\n";

    // 1) Create the players
    std::cout << "[GameState::CTOR] settings.numPlayers = " << settings.numPlayers << "\n";
    players.reserve(settings.numPlayers);
    for (int i = 0; i < settings.numPlayers; ++i) {
        players.emplace_back("Player " + std::to_string(i + 1), settings.startingMoney);
        std::cout << "  -> Created " << players.back().getName()
                  << " with starting money = " << settings.startingMoney << "\n";
    }

    // 2) Load a known local font (Troska.ttf) instead of scanning system paths
    std::cout << "[GameState::CTOR] Loading local Casino.ttf...\n";
    bool loaded = font.loadFromFile("src/assets/font/Casino.ttf");
    if (!loaded) {
        std::cerr << "[GameState::CTOR] ERROR: Could not load src/assets/font/Casino.ttf\n";
        // handle error or exit if you want
    } else {
        std::cout << "[GameState::CTOR] SUCCESS: Loaded Casino.ttf.\n";
    }

    // 3) Now that font is loaded, properly construct the 3 buttons with real data
    hitButton = Button(
        sf::Vector2f(50.f, 600.f),
        sf::Vector2f(150.f, 50.f),
        "HIT",
        font,
        sf::Color::White,
        sf::Color::Yellow
    );
    standButton = Button(
        sf::Vector2f(220.f, 600.f),
        sf::Vector2f(150.f, 50.f),
        "STAND",
        font,
        sf::Color::White,
        sf::Color::Yellow
    );
    wildcardButton = Button(
        sf::Vector2f(390.f, 600.f),
        sf::Vector2f(150.f, 50.f),
        "WILDCARD",
        font,
        sf::Color::White,
        sf::Color::Yellow
    );

    // 4) Shuffle main deck, and wildcard deck if enabled
    std::cout << "[GameState::CTOR] Shuffling main deck...\n";
    deck.shuffle();
    if (settings.wildcardEnabled) {
        std::cout << "[GameState::CTOR] Wildcards enabled; shuffling wildcard deck...\n";
        wildcardDeck.shuffle();
    }

    // 5) Setup SFML text objects
    roundInfoText.setFont(font);
    roundInfoText.setCharacterSize(24);
    roundInfoText.setFillColor(sf::Color::White);
    roundInfoText.setPosition(50.f, 50.f);

    wildcardInfoText.setFont(font);
    wildcardInfoText.setCharacterSize(24);
    wildcardInfoText.setFillColor(sf::Color::Cyan);
    wildcardInfoText.setPosition(50.f, 100.f);

    messageText.setFont(font);
    messageText.setCharacterSize(28);
    messageText.setFillColor(sf::Color::Yellow);
    messageText.setPosition(50.f, 150.f);

    // 6) Start the first round
    std::cout << "[GameState::CTOR] About to call startNewRound()...\n";
    startNewRound();
    std::cout << "[GameState::CTOR] End constructor.\n\n";
}

void GameState::startNewRound() {
    std::cout << "[GameState::startNewRound] Clearing hands/wildcards, resetting decks...\n";
    roundConcluded = false;

    // Clear previous hands, bets, etc.
    for (auto& p : players) {
        p.clearHand();
        p.clearWildcards();
    }

    // Reset & shuffle main deck
    deck.reset();
    if (settings.wildcardEnabled) {
        wildcardDeck.reset();
    }

    std::cout << "[GameState::startNewRound] Dealing initial cards...\n";
    dealInitialCards();

    if (settings.wildcardEnabled) {
        std::cout << "[GameState::startNewRound] Dealing wildcards...\n";
        dealWildcards();
    }

    currentPlayerIndex = 0;
    roundInProgress = true;
    messageText.setString("");

    updateLabels();
    std::cout << "[GameState::startNewRound] Done.\n";
}

void GameState::dealInitialCards() {
    for (int i = 0; i < 2; ++i) {
        for (auto& p : players) {
            Card c = deck.draw();
            p.addCard(std::make_shared<Card>(c));
        }
    }
    std::cout << "[GameState::dealInitialCards] Dealt 2 cards to each player.\n";
}

void GameState::dealWildcards() {
    for (auto& p : players) {
        auto w = wildcardDeck.draw();
        p.addWildcard(w);
        std::cout << "  -> " << p.getName() << " got wildcard [" << w->getName() << "]\n";
    }
    std::cout << "[GameState::dealWildcards] Dealt 1 wildcard to each player.\n";
}

void GameState::handleInput() {
    sf::Event event;
    while (window.pollEvent(event)) {
        // For logging
        // This might spam the console, but helps debug
        // std::cout << "[handleInput] Event type: " << event.type << "\n";

        if (event.type == sf::Event::Closed) {
            std::cout << "  -> Window closed event\n";
            window.close();
        }
        else if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                
                // HIT
                if (hitButton.isMouseOver(mousePos) && roundInProgress && !roundConcluded) {
                    std::cout << "  -> HIT button clicked\n";
                    Player& current = players[currentPlayerIndex];
                    Card c = deck.draw();
                    current.addCard(std::make_shared<Card>(c));
                    if (current.isBusted()) {
                        std::cout << "  -> " << current.getName() << " BUSTED!\n";
                        messageText.setString(current.getName() + " BUSTED!");
                    }
                    updateLabels();
                }
                // STAND
                else if (standButton.isMouseOver(mousePos) && roundInProgress && !roundConcluded) {
                    std::cout << "  -> STAND button clicked\n";
                    messageText.setString(players[currentPlayerIndex].getName() + " stands.");
                    nextPlayer();
                }
                // WILDCARD
                else if (wildcardButton.isMouseOver(mousePos) && roundInProgress && !roundConcluded) {
                    std::cout << "  -> WILDCARD button clicked\n";
                    Player& current = players[currentPlayerIndex];
                    if (current.hasWildcards()) {
                        // Use the first wildcard
                        if (current.useWildcard(0, players)) {
                            std::cout << "  -> " << current.getName() << " used wildcard [index 0]\n";
                            messageText.setString(current.getName() + " used a wildcard!");
                            updateLabels();
                        }
                    } else {
                        std::cout << "  -> " << current.getName() << " has no wildcards.\n";
                        messageText.setString("No wildcards to use!");
                    }
                }
            }
        }
        else if (event.type == sf::Event::MouseMoved) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            hitButton.setHighlight(hitButton.isMouseOver(mousePos));
            standButton.setHighlight(standButton.isMouseOver(mousePos));
            wildcardButton.setHighlight(wildcardButton.isMouseOver(mousePos));
        }
    }
}

void GameState::update() {
    // Called once per frame (60 fps, etc.)
    if (!roundInProgress || roundConcluded) {
        return;
    }

    Player& current = players[currentPlayerIndex];
    if (current.isBusted() || current.calculateHandTotal() == 21) {
        std::cout << "[GameState::update] " << current.getName()
                  << " done or at 21 -> nextPlayer()\n";
        nextPlayer();
    }

    if (currentPlayerIndex >= (int)players.size()) {
        concludeRound();
    }

    float deltaTime = 1.f / 60.f;
    hitButton.update(deltaTime);
    standButton.update(deltaTime);
    wildcardButton.update(deltaTime);
}

void GameState::nextPlayer() {
    std::cout << "[GameState::nextPlayer] currentPlayerIndex was " 
              << currentPlayerIndex << ", incrementing...\n";
    currentPlayerIndex++;
    if (currentPlayerIndex >= (int)players.size()) {
        std::cout << "  -> currentPlayerIndex == players.size(), concludeRound\n";
        concludeRound();
    } else {
        std::cout << "  -> new currentPlayerIndex = " << currentPlayerIndex << "\n";
        messageText.setString("");
        updateLabels();
    }
}

void GameState::concludeRound() {
    std::cout << "[GameState::concludeRound] Start.\n";
    roundInProgress = false;
    roundConcluded = true;

    int bestScore = -1;
    std::vector<int> winners;
    for (int i = 0; i < (int)players.size(); ++i) {
        if (!players[i].isBusted()) {
            int score = players[i].calculateHandTotal();
            std::cout << "  -> " << players[i].getName()
                      << " final total = " << score << "\n";
            if (score > bestScore) {
                bestScore = score;
                winners.clear();
                winners.push_back(i);
            }
            else if (score == bestScore) {
                winners.push_back(i);
            }
        } else {
            std::cout << "  -> " << players[i].getName() << " is busted.\n";
        }
    }

    if (winners.empty()) {
        messageText.setString("Everyone busted. No winners!");
        std::cout << "  -> Everyone busted!\n";
    } else {
        std::string winnerNames;
        for (auto w : winners) {
            winnerNames += players[w].getName() + " ";
        }
        messageText.setString("Winner(s): " + winnerNames + " with " + std::to_string(bestScore));
        std::cout << "  -> Winner(s): " << winnerNames << "with " << bestScore << "\n";
    }
    std::cout << "[GameState::concludeRound] End.\n";
}

void GameState::updateLabels() {
    if (currentPlayerIndex < (int)players.size()) {
        Player& current = players[currentPlayerIndex];
        int total = current.calculateHandTotal();
        roundInfoText.setString(current.getName() + " - Total: " + std::to_string(total));
        
        if (current.hasWildcards()) {
            std::string wText = "Wildcards: ";
            for (auto& w : current.getWildcards()) {
                wText += "[" + w->getName() + "] ";
            }
            wildcardInfoText.setString(wText);
        } else {
            wildcardInfoText.setString("No wildcards available.");
        }
    }
    else {
        roundInfoText.setString("All players done.");
        wildcardInfoText.setString("");
    }
}

void GameState::render() {
    window.clear(sf::Color(30, 30, 30));

    hitButton.draw(window);
    standButton.draw(window);
    wildcardButton.draw(window);

    window.draw(roundInfoText);
    window.draw(wildcardInfoText);
    window.draw(messageText);

    window.display();
}
