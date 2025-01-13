#include "GameState.hpp"
#include <iostream>
#include <cmath>
#include <algorithm>

// We'll keep the same card geometry
static const float CARD_WIDTH = 80.f;
static const float CARD_HEIGHT = 120.f;

// Helper to create a fancy card shape
sf::RectangleShape createCardShape(const sf::Vector2f& position,
                                   const sf::Color& topColor,
                                   const sf::Color& bottomColor)
{
    sf::RectangleShape shape;
    shape.setSize(sf::Vector2f(CARD_WIDTH, CARD_HEIGHT));
    shape.setPosition(position);
    shape.setOutlineThickness(2.f);
    shape.setOutlineColor(sf::Color::Black);
    // For simplicity, we just set a single fill color (topColor).
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
    , pot(0.f)  // <--- Initialize pot to 0
    // We'll fix the button fonts below
    , hitButton(sf::Vector2f(0.f,0.f), sf::Vector2f(120.f,50.f), "HIT", sf::Font())
    , standButton(sf::Vector2f(0.f,0.f), sf::Vector2f(120.f,50.f), "STAND", sf::Font())
    , wildcardButton(sf::Vector2f(0.f,0.f), sf::Vector2f(120.f,50.f), "WILDCARD", sf::Font())
    , addBetButton(sf::Vector2f(0.f,0.f), sf::Vector2f(120.f,50.f), "BET $10", sf::Font())  // <--- new
{
    std::cout << "[GameState::CTOR] Begin\n";
    std::cout << "  -> Number of players: " << settings.numPlayers << "\n";

    // Create players
    players.reserve(settings.numPlayers);
    for (int i = 0; i < settings.numPlayers; ++i) {
        players.emplace_back("Player " + std::to_string(i+1), settings.startingMoney);
        std::cout << "     Created " << players.back().getName() 
                  << " with $" << settings.startingMoney << "\n";
    }

    // 1) Load a basic system font (e.g. "arial.ttf")
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
    sf::Vector2f betPos(wildcardPos.x + buttonSpacing, buttonY);  // <--- for the "BET $10" button

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

    // New "BET $10" button
    addBetButton = Button(
        betPos,
        sf::Vector2f(120.f, 50.f),
        "BET $10",
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

    // Add a potText to show the pot near the top-left as well
    potText.setFont(font);
    potText.setCharacterSize(24);
    potText.setFillColor(sf::Color::Magenta);
    potText.setPosition(50.f, 180.f);

    startNewRound();
    std::cout << "[GameState::CTOR] End\n";
}

void GameState::startNewRound() {
    std::cout << "[GameState::startNewRound] Clear + reset\n";
    roundConcluded = false;
    waitingForReplay = false;
    
    // Reset pot to 0 each round
    pot = 0.f;  

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
        else if (waitingForReplay && event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::R) {
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
                // BET $10
                else if (addBetButton.isMouseOver(mousePos) && roundInProgress && !roundConcluded) {
                    Player& current = players[currentPlayerIndex];
                    float betAmount = 10.f;
                    if (current.canBet(betAmount)) {
                        current.placeBet(betAmount); 
                        pot += betAmount;
                        messageText.setString(current.getName() + " bet $" + std::to_string((int)betAmount));
                    } else {
                        messageText.setString("Not enough funds to bet $10!");
                    }
                    updateLabels();
                }
            }
        }
        else if (event.type == sf::Event::MouseMoved) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            hitButton.setHighlight(hitButton.isMouseOver(mousePos));
            standButton.setHighlight(standButton.isMouseOver(mousePos));
            wildcardButton.setHighlight(wildcardButton.isMouseOver(mousePos));
            addBetButton.setHighlight(addBetButton.isMouseOver(mousePos));
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

    // Update buttons
    float dt = 1.f / 60.f;
    hitButton.update(dt);
    standButton.update(dt);
    wildcardButton.update(dt);
    addBetButton.update(dt);
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

// In concludeRound, pay out the pot to the winner(s)
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
        // Everyone busted
        messageText.setString("Everyone busted. No winners! Pot lost. Press R to replay.");
        // pot is effectively lost to the house
        pot = 0.f;
    } else {
        std::string names;
        for (auto w : winners) {
            names += players[w].getName() + " ";
        }
        // If there's multiple winners, they share the pot equally
        float share = pot / winners.size();
        for (auto w : winners) {
            players[w].addWinnings(share);
        }

        messageText.setString("Winner(s): " + names
                             + " with " + std::to_string(players[winners[0]].calculateHandTotal())
                             + " and split $" + std::to_string((int)pot)
                             + "! Press R to replay.");
        pot = 0.f; // pot reset after payout
    }

    waitingForReplay = true;
}

void GameState::updateLabels() {
    // Show the pot amount
    potText.setString("Pot: $" + std::to_string((int)pot));

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

    // swirl background
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

    // Buttons
    hitButton.draw(window);
    standButton.draw(window);
    wildcardButton.draw(window);
    addBetButton.draw(window); // new

    // Info texts
    window.draw(roundInfoText);
    window.draw(wildcardInfoText);
    window.draw(messageText);

    // Show pot text
    window.draw(potText);

    // Circular layout for players
    float radius = 250.f; 
    float angleStep = 360.f / players.size();

    for (size_t i = 0; i < players.size(); ++i) {
        Player& p = players[i];
        float angleDeg = angleStep * i - 90; // start top
        float angleRad = angleDeg * 3.14159f / 180.f;
        float px = centerX + radius * std::cos(angleRad);
        float py = centerY + radius * std::sin(angleRad);

        // highlight if current
        if ((int)i == currentPlayerIndex) {
            sf::CircleShape highlight(60.f);
            highlight.setFillColor(sf::Color(255, 255, 0, 50));
            highlight.setOrigin(highlight.getRadius(), highlight.getRadius());
            highlight.setPosition(px, py);
            window.draw(highlight);
        }

        // If busted, color name differently
        sf::Text nameText;
        nameText.setFont(font);
        nameText.setCharacterSize(24);
        if (p.isBusted()) {
            nameText.setString(p.getName() + " (BUSTED!)");
            nameText.setFillColor(sf::Color(255, 100, 100));
        } else {
            nameText.setString(p.getName());
            nameText.setFillColor(sf::Color::White);
        }
        sf::FloatRect nb = nameText.getLocalBounds();
        nameText.setOrigin(nb.width/2.f, nb.height);
        nameText.setPosition(px, py - 70.f);
        window.draw(nameText);

        // Draw their cards
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

        // If they have wildcards, show them below
        if (p.hasWildcards()) {
            sf::Text wcLabel;
            wcLabel.setFont(font);
            wcLabel.setCharacterSize(18);
            wcLabel.setFillColor(sf::Color::Yellow);
            std::string wcSummary;
            for (auto& w : p.getWildcards()) {
                wcSummary += w->getName() + " ";
            }
            wcLabel.setString("WC: " + wcSummary);
            sf::FloatRect wb = wcLabel.getLocalBounds();
            wcLabel.setOrigin(wb.width/2.f, 0.f);
            wcLabel.setPosition(px, py + 70.f);
            window.draw(wcLabel);
        }
    }

    window.display();
}
