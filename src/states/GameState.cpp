#include "GameState.hpp"
#include <iostream>
#include <cmath>
#include <algorithm>

// For clarity, define card geometry
static const float CARD_WIDTH = 80.f;
static const float CARD_HEIGHT = 120.f;

// Helper for fancy card shape
sf::RectangleShape createCardShape(const sf::Vector2f& position,
                                   const sf::Color& topColor,
                                   const sf::Color& bottomColor)
{
    sf::RectangleShape shape;
    shape.setSize(sf::Vector2f(CARD_WIDTH, CARD_HEIGHT));
    shape.setPosition(position);
    shape.setOutlineThickness(2.f);
    shape.setOutlineColor(sf::Color::Black);
    shape.setFillColor(topColor);
    return shape;
}

GameState::GameState(sf::RenderWindow& window, const GameSettings& settings)
    : State(window)
    , settings(settings)
    , currentPlayerIndex(0)
    , roundInProgress(false)
    , roundConcluded(false)
    , waitingForReplay(false) 
    // We'll fix button fonts below
    , hitButton(sf::Vector2f(0.f,0.f), sf::Vector2f(120.f, 50.f), "HIT", sf::Font())
    , standButton(sf::Vector2f(0.f,0.f), sf::Vector2f(120.f, 50.f), "STAND", sf::Font())
    , wildcardButton(sf::Vector2f(0.f,0.f), sf::Vector2f(120.f, 50.f), "WILDCARD", sf::Font())
{
    std::cout << "[GameState::CTOR] Begin\n";
    std::cout << "  -> Number of players: " << settings.numPlayers << "\n";

    // Create players
    players.reserve(settings.numPlayers);
    for (int i = 0; i < settings.numPlayers; ++i) {
        players.emplace_back("Player " + std::to_string(i + 1), settings.startingMoney);
        std::cout << "     Created " << players.back().getName()
                  << " with $" << settings.startingMoney << "\n";
    }

    // 1) Load a basic system font
    std::vector<std::string> systemFonts = {
        "arial.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "C:/Windows/Fonts/Arial.ttf"
    };
    bool loaded = false;
    for (auto& path : systemFonts) {
        if (font.loadFromFile(path)) {
            loaded = true;
            std::cout << "[GameState] Loaded system font: " << path << "\n";
            break;
        }
    }
    if (!loaded) {
        std::cerr << "[GameState::CTOR] ERROR: Could not load a system font.\n";
    }

    // 2) Position the buttons along the bottom, spaced out horizontally
    const float buttonY = window.getSize().y - 70.f;
    const float buttonSpacing = 200.f;
    sf::Vector2f hitPos(50.f, buttonY);
    sf::Vector2f standPos(hitPos.x + buttonSpacing, buttonY);
    sf::Vector2f wildcardPos(standPos.x + buttonSpacing, buttonY);

    // Re-initialize the buttons with real font & new positions
    hitButton = Button(
        hitPos,
        sf::Vector2f(120.f, 50.f),
        "HIT",
        font,
        sf::Color::White,
        sf::Color::Yellow
    );
    standButton = Button(
        standPos,
        sf::Vector2f(120.f, 50.f),
        "STAND",
        font,
        sf::Color::White,
        sf::Color::Yellow
    );
    wildcardButton = Button(
        wildcardPos,
        sf::Vector2f(120.f, 50.f),
        "WILDCARD",
        font,
        sf::Color::White,
        sf::Color::Yellow
    );

    // Shuffle decks
    deck.shuffle();
    if (settings.wildcardEnabled) {
        wildcardDeck.shuffle();
    }

    // Setup text objects
    roundInfoText.setFont(font);
    roundInfoText.setCharacterSize(24);
    roundInfoText.setFillColor(sf::Color::White);
    roundInfoText.setPosition(50.f, 50.f);

    wildcardInfoText.setFont(font);
    wildcardInfoText.setCharacterSize(24);
    wildcardInfoText.setFillColor(sf::Color::Cyan);
    wildcardInfoText.setPosition(50.f, 90.f);

    messageText.setFont(font);
    messageText.setCharacterSize(26);
    messageText.setFillColor(sf::Color::Yellow);
    messageText.setPosition(50.f, 140.f);

    // Start round
    startNewRound();
    std::cout << "[GameState::CTOR] End\n";
}

void GameState::startNewRound() {
    std::cout << "[GameState::startNewRound] Clear + reset\n";
    roundConcluded = false;
    waitingForReplay = false;

    for (auto& p : players) {
        p.clearHand();
        p.clearWildcards();
    }

    deck.reset();
    if (settings.wildcardEnabled) {
        wildcardDeck.reset();
    }

    dealInitialCards();
    if (settings.wildcardEnabled) {
        dealWildcards();
    }

    currentPlayerIndex = 0;
    roundInProgress = true;
    messageText.setString("");

    updateLabels();
}

void GameState::dealInitialCards() {
    for (int i = 0; i < 2; ++i) {
        for (auto& p : players) {
            Card c = deck.draw();
            p.addCard(std::make_shared<Card>(c));
        }
    }
    std::cout << "[GameState::dealInitialCards] 2 cards each\n";
}

void GameState::dealWildcards() {
    for (auto& p : players) {
        auto w = wildcardDeck.draw();
        p.addWildcard(w);
        std::cout << "  -> " << p.getName() << " got wildcard [" << w->getName() << "]\n";
    }
}

void GameState::handleInput() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        // If concluded, pressing 'R' replays
        else if (waitingForReplay && event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::R) {
                std::cout << "[GameState] 'R' pressed -> startNewRound()\n";
                startNewRound();
            }
        }
        else if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                // HIT
                if (hitButton.isMouseOver(mousePos) && roundInProgress && !roundConcluded) {
                    Player& current = players[currentPlayerIndex];
                    Card c = deck.draw();
                    current.addCard(std::make_shared<Card>(c));
                    if (current.isBusted()) {
                        messageText.setString(current.getName() + " BUSTED!");
                    }
                    updateLabels();
                }
                // STAND
                else if (standButton.isMouseOver(mousePos) && roundInProgress && !roundConcluded) {
                    messageText.setString(players[currentPlayerIndex].getName() + " stands.");
                    nextPlayer();
                }
                // WILDCARD
                else if (wildcardButton.isMouseOver(mousePos) && roundInProgress && !roundConcluded) {
                    Player& current = players[currentPlayerIndex];
                    if (current.hasWildcards()) {
                        if (current.useWildcard(0, players)) {
                            messageText.setString(current.getName() + " used a wildcard!");
                            updateLabels();
                        }
                    } else {
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
    if (!roundInProgress || roundConcluded) return;

    Player& current = players[currentPlayerIndex];
    int total = current.calculateHandTotal();
    if (current.isBusted() || total == 21) {
        nextPlayer();
    }
    if (currentPlayerIndex >= (int)players.size()) {
        concludeRound();
    }

    float dt = 1.f / 60.f;
    hitButton.update(dt);
    standButton.update(dt);
    wildcardButton.update(dt);
}

void GameState::nextPlayer() {
    currentPlayerIndex++;
    if (currentPlayerIndex >= (int)players.size()) {
        concludeRound();
    } else {
        messageText.setString("");
        updateLabels();
    }
}

void GameState::concludeRound() {
    roundInProgress = false;
    roundConcluded = true;

    int bestScore = -1;
    std::vector<int> winners;
    for (int i = 0; i < (int)players.size(); ++i) {
        if (!players[i].isBusted()) {
            int score = players[i].calculateHandTotal();
            if (score > bestScore) {
                bestScore = score;
                winners.clear();
                winners.push_back(i);
            }
            else if (score == bestScore) {
                winners.push_back(i);
            }
        }
    }
    if (winners.empty()) {
        messageText.setString("Everyone busted. No winners! Press R to replay.");
    } else {
        std::string names;
        for (auto w : winners) {
            names += players[w].getName() + " ";
        }
        messageText.setString("Winner(s): " + names
                             + " with " + std::to_string(players[winners[0]].calculateHandTotal())
                             + ". Press R to replay!");
    }

    waitingForReplay = true;
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
    window.clear(sf::Color(20, 100, 20));  // deeper green

    // swirling background
    float centerX = window.getSize().x / 2.f;
    float centerY = window.getSize().y / 2.f;
    for (int i = 0; i < 8; ++i) {
        sf::CircleShape swirl(100.f + i * 30.f, 64);
        swirl.setFillColor(sf::Color(0, 0, 0, 0));
        swirl.setOutlineThickness(2.f);
        swirl.setOutlineColor(sf::Color(0, 150 - i * 15, 0, 100));
        swirl.setOrigin(swirl.getRadius(), swirl.getRadius());
        swirl.setPosition(centerX, centerY);
        swirl.rotate(i * 15.f);
        window.draw(swirl);
    }

    // buttons
    hitButton.draw(window);
    standButton.draw(window);
    wildcardButton.draw(window);

    // text overlays
    window.draw(roundInfoText);
    window.draw(wildcardInfoText);
    window.draw(messageText);

    // circle layout for players
    float radius = 250.f; 
    float angleStep = 360.f / players.size();

    // Flicker for BUST: time-based
    long long ms = animationClock.getElapsedTime().asMilliseconds();
    float flickerTime = std::fmod(static_cast<float>(ms) / 150.f, 2.f);
    sf::Color bustColor = (flickerTime < 1.f)
                            ? sf::Color(255, 50, 50)   // bright red
                            : sf::Color(255, 165, 0); // orange

    for (size_t i = 0; i < players.size(); ++i) {
        Player& p = players[i];
        float angleDeg = angleStep * i - 90.f;
        float angleRad = angleDeg * 3.14159f / 180.f;
        float px = centerX + radius * std::cos(angleRad);
        float py = centerY + radius * std::sin(angleRad);

        // highlight current
        if ((int)i == currentPlayerIndex) {
            sf::CircleShape highlight(60.f);
            highlight.setFillColor(sf::Color(255, 255, 0, 50));
            highlight.setOrigin(highlight.getRadius(), highlight.getRadius());
            highlight.setPosition(px, py);
            window.draw(highlight);
        }

        // name text
        sf::Text nameText;
        nameText.setFont(font);
        nameText.setCharacterSize(24);

        // If busted, append " (BUSTED!)" to name and flicker color
        if (p.isBusted()) {
            std::string bustedName = p.getName() + " (BUSTED!)";
            nameText.setString(bustedName);
            nameText.setFillColor(bustColor);
        }
        else {
            nameText.setString(p.getName());
            nameText.setFillColor(sf::Color::White);
        }

        sf::FloatRect nb = nameText.getLocalBounds();
        nameText.setOrigin(nb.width/2.f, nb.height);
        nameText.setPosition(px, py - 70.f);
        window.draw(nameText);

        // cards
        float cardOffsetX = px - (CARD_WIDTH + 10.f)*(p.getHand().size()/2.f);
        float cardOffsetY = py - CARD_HEIGHT/2.f;
        for (size_t cIndex = 0; cIndex < p.getHand().size(); ++cIndex) {
            auto& card = p.getHand()[cIndex];
            float cx = cardOffsetX + cIndex*(CARD_WIDTH + 10.f);
            float cy = cardOffsetY;

            sf::RectangleShape cardShape = createCardShape(
                sf::Vector2f(cx, cy),
                sf::Color(255, 255, 255),
                sf::Color(200, 200, 200)
            );
            window.draw(cardShape);

            sf::Text cardText;
            cardText.setFont(font);
            cardText.setCharacterSize(20);
            cardText.setFillColor(sf::Color::Black);
            cardText.setString(card->toString());
            sf::FloatRect cb = cardText.getLocalBounds();
            cardText.setOrigin(cb.width/2.f, cb.height/2.f);
            cardText.setPosition(cx + CARD_WIDTH/2.f, cy + CARD_HEIGHT/2.f);
            window.draw(cardText);
        }

        // wildcards
        if (p.hasWildcards()) {
            sf::Text wcLabel;
            wcLabel.setFont(font);
            wcLabel.setCharacterSize(18);
            wcLabel.setFillColor(sf::Color::Yellow);
            std::string wildcardSummary;
            for (auto& w : p.getWildcards()) {
                wildcardSummary += w->getName() + " ";
            }
            wcLabel.setString("WC: " + wildcardSummary);
            sf::FloatRect wb = wcLabel.getLocalBounds();
            wcLabel.setOrigin(wb.width/2.f, 0.f);
            wcLabel.setPosition(px, py + 70.f);
            window.draw(wcLabel);
        }
    }

    window.display();
}
