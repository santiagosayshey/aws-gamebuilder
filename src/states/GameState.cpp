#include "states/GameState.hpp"
#include <iostream>
#include <cmath>
#include <algorithm>

GameState::GameState(sf::RenderWindow &window, const GameSettings &settings)
    : State(window), settings(settings), pot(0.f), currentPlayerIndex(0), roundInProgress(false), roundConcluded(false), waitingForReplay(false), hitButton(sf::Vector2f(0.f, 0.f), sf::Vector2f(120.f, 50.f), "HIT", sf::Font()), standButton(sf::Vector2f(0.f, 0.f), sf::Vector2f(120.f, 50.f), "STAND", sf::Font()), doubleDownButton(sf::Vector2f(0.f, 0.f), sf::Vector2f(120.f, 50.f), "DOUBLE DOWN", sf::Font()), wildcardButton(sf::Vector2f(0.f, 0.f), sf::Vector2f(120.f, 50.f), "WILDCARD", sf::Font()), addBetButton(sf::Vector2f(0.f, 0.f), sf::Vector2f(120.f, 50.f), "BET $10", sf::Font())
{
    // Load font
    std::vector<std::string> systemFonts = {
        "arial.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "C:/Windows/Fonts/Arial.ttf"};

    bool fontLoaded = false;
    for (const auto &path : systemFonts)
    {
        if (font.loadFromFile(path))
        {
            fontLoaded = true;
            break;
        }
    }
    if (!fontLoaded)
    {
        std::cerr << "Failed to load any system font\n";
    }

    // Initialize players
    players.reserve(settings.numPlayers);
    for (int i = 0; i < settings.numPlayers; ++i)
    {
        players.emplace_back("Player " + std::to_string(i + 1), settings.startingMoney);
    }

    // Position buttons along bottom
    const float buttonY = window.getSize().y - 70.f;
    const float buttonSpacing = 180.f;                                        // Increased spacing
    const float totalWidth = (120.f * 2) + (180.f * 2) + (3 * buttonSpacing); // Total width of all buttons
    float startX = (window.getSize().x - totalWidth) / 2.f;                   // Center point
    float currentX = startX;

    hitButton = Button(
        sf::Vector2f(currentX, buttonY),
        sf::Vector2f(120.f, 50.f),
        "HIT",
        font,
        sf::Color::White,
        sf::Color::Yellow);
    currentX += 120.f + buttonSpacing;

    standButton = Button(
        sf::Vector2f(currentX, buttonY),
        sf::Vector2f(120.f, 50.f),
        "STAND",
        font,
        sf::Color::White,
        sf::Color::Yellow);
    currentX += 120.f + buttonSpacing;

    doubleDownButton = Button(
        sf::Vector2f(currentX, buttonY),
        sf::Vector2f(180.f, 50.f), // Wider button
        "DOUBLE DOWN",
        font,
        sf::Color::White,
        sf::Color::Yellow);
    currentX += 180.f + buttonSpacing;

    wildcardButton = Button(
        sf::Vector2f(currentX, buttonY),
        sf::Vector2f(180.f, 50.f), // Wider button
        "WILDCARD",
        font,
        sf::Color::White,
        sf::Color::Yellow);
    currentX += 180.f + buttonSpacing;

    addBetButton = Button(
        sf::Vector2f(currentX, buttonY),
        sf::Vector2f(180.f, 50.f), // Wider button
        "BET $10",
        font,
        sf::Color::White,
        sf::Color::Yellow);

    // Initialize text displays
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
    messageText.setPosition(50.f, 130.f);

    potText.setFont(font);
    potText.setCharacterSize(24);
    potText.setFillColor(sf::Color::Magenta);
    potText.setPosition(50.f, 170.f);

    startNewRound();
}

void GameState::startNewRound()
{
    roundConcluded = false;
    waitingForReplay = false;
    pot = 0.f;

    // Reset all players
    for (auto &p : players)
    {
        p.clearHand();
        p.clearWildcards();
    }

    // Reset and shuffle decks
    deck.reset();
    deck.shuffle();
    if (settings.wildcardEnabled)
    {
        wildcardDeck.reset();
        wildcardDeck.shuffle();
    }

    dealInitialCards();
    if (settings.wildcardEnabled)
    {
        dealWildcards();
    }

    currentPlayerIndex = 0;
    roundInProgress = true;
    messageText.setString("");

    updateLabels();
}

void GameState::dealInitialCards()
{
    for (int i = 0; i < 2; ++i)
    {
        for (auto &p : players)
        {
            Card c = deck.draw();
            p.addCard(std::make_shared<Card>(c));
        }
    }
}

void GameState::dealWildcards()
{
    for (auto &p : players)
    {
        auto w = wildcardDeck.draw();
        p.addWildcard(w);
    }
}

void GameState::handleInput()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }
        else if (waitingForReplay && event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::R)
            {
                startNewRound();
            }
        }
        else if (event.type == sf::Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                if (roundInProgress && !roundConcluded)
                {
                    Player &current = players[currentPlayerIndex];

                    if (hitButton.isMouseOver(mousePos))
                    {
                        if (current.hasForesight() == true)
                        {
                            Card c = deck.peek();
                            int val = c.getValue();
                            std::string val_str;
                            if (val == 1)
                            { // Ace
                                val_str = "Ace";
                            }
                            else if (val > 10 && val < 14)
                            { // Face cards
                                val = 10;
                                val_str = std::to_string(val);
                            }
                            else
                            {
                                val_str = std::to_string(val);
                            };
                            messageText.setString("Next card is " + val_str + ". Click hit or double down to confirm the hit");
                            current.setForesightFalse();
                        }

                        else
                        {
                            Card c = deck.draw();
                            current.addCard(std::make_shared<Card>(c));
                            if (current.isBusted())
                            {
                                messageText.setString(current.getName() + " BUSTED!");
                            }
                        }

                        updateLabels();
                    }
                    else if (standButton.isMouseOver(mousePos))
                    {
                        messageText.setString(current.getName() + " stands.");
                        nextPlayer();
                    }
                    else if (wildcardButton.isMouseOver(mousePos))
                    {
                        if (current.hasWildcards())
                        {
                            if (current.useWildcard(0, players))
                            {
                                messageText.setString(current.getName() + " used a wildcard!");
                                updateLabels();
                            }
                        }
                        else
                        {
                            messageText.setString("No wildcards available!");
                        }
                    }
                    else if (addBetButton.isMouseOver(mousePos))
                    {
                        float betAmount = 10.f;
                        if (current.canBet(betAmount))
                        {
                            current.placeBet(betAmount);
                            pot += betAmount;
                            messageText.setString(current.getName() + " bet $" + std::to_string((int)betAmount));
                        }
                        else
                        {
                            messageText.setString("Not enough funds to bet $10!");
                        }
                        updateLabels();
                    }
                    else if (doubleDownButton.isMouseOver(mousePos))
                    {
                        if (current.canBet(pot))
                        {
                            float betAmount = pot;
                            current.doubleBet(betAmount);
                            pot += betAmount;
                            messageText.setString(current.getName() + " doubled down.");

                            Card c = deck.draw();
                            current.addCard(std::make_shared<Card>(c));
                            if (current.isBusted())
                            {
                                messageText.setString(current.getName() + " BUSTED!");
                            }
                            updateLabels();
                        }
                        else
                        {
                            messageText.setString("Not enough funds");
                        }
                    }
                }
            }
        }
        else if (event.type == sf::Event::MouseMoved)
        {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            hitButton.setHighlight(hitButton.isMouseOver(mousePos));
            standButton.setHighlight(standButton.isMouseOver(mousePos));
            wildcardButton.setHighlight(wildcardButton.isMouseOver(mousePos));
            addBetButton.setHighlight(addBetButton.isMouseOver(mousePos));
        }
    }
}

void GameState::update()
{
    if (!roundInProgress || roundConcluded)
        return;

    Player &current = players[currentPlayerIndex];
    int total = current.calculateHandTotal();
    if (current.isBusted() || total == 21)
    {
        nextPlayer();
    }

    // Update UI elements
    float dt = 1.f / 60.f;
    hitButton.update(dt);
    standButton.update(dt);
    wildcardButton.update(dt);
    addBetButton.update(dt);
}

void GameState::nextPlayer()
{
    currentPlayerIndex++;
    if (currentPlayerIndex >= (int)players.size())
    {
        concludeRound();
    }
    else
    {
        messageText.setString("");
        updateLabels();
    }
}

void GameState::concludeRound()
{
    roundInProgress = false;
    roundConcluded = true;

    int bestScore = -1;
    std::vector<int> winners;

    // Find winners
    for (int i = 0; i < (int)players.size(); ++i)
    {
        if (!players[i].isBusted())
        {
            int score = players[i].calculateHandTotal();
            if (score > bestScore)
            {
                bestScore = score;
                winners.clear();
                winners.push_back(i);
            }
            else if (score == bestScore)
            {
                winners.push_back(i);
            }
        }
    }

    if (winners.empty())
    {
        messageText.setString("Everyone busted! Press R to replay.");
        pot = 0.f; // House takes the pot
    }
    else
    {
        std::string names;
        float share = pot / winners.size();

        for (auto w : winners)
        {
            names += players[w].getName() + " ";
            players[w].addWinnings(share);
        }

        messageText.setString("Winner(s): " + names +
                              "with " + std::to_string(bestScore) +
                              " splitting $" + std::to_string((int)pot) +
                              "! Press R to replay.");
        pot = 0.f;
    }

    waitingForReplay = true;
}

void GameState::updateLabels()
{
    potText.setString("Pot: $" + std::to_string((int)pot));

    if (currentPlayerIndex < (int)players.size())
    {
        Player &current = players[currentPlayerIndex];
        roundInfoText.setString(current.getName() + " - Total: " +
                                std::to_string(current.calculateHandTotal()));

        if (current.hasWildcards())
        {
            std::string wText = "Wildcards: ";
            for (const auto &w : current.getWildcards())
            {
                wText += "[" + w->getName() + "] ";
            }
            wildcardInfoText.setString(wText);
        }
        else
        {
            wildcardInfoText.setString("No wildcards available.");
        }
    }
    else
    {
        roundInfoText.setString("All players done.");
        wildcardInfoText.setString("");
    }
}

void GameState::render()
{
    window.clear(sf::Color(0, 60, 0)); // Darker base green

    // Draw decorative corner circles
    const float centerX = window.getSize().x / 2.f;
    const float centerY = window.getSize().y / 2.f;
    static float time = 0.0f;
    time += 1.f / 60.f; // Animation time

    // Draw large decorative circles in corners
    std::vector<sf::Vector2f> cornerPositions = {
        {0, 0},                                  // Top-left
        {window.getSize().x, 0},                 // Top-right
        {0, window.getSize().y},                 // Bottom-left
        {window.getSize().x, window.getSize().y} // Bottom-right
    };

    for (size_t i = 0; i < cornerPositions.size(); ++i)
    {
        sf::CircleShape corner(200.f);
        corner.setFillColor(sf::Color(0, 80, 0, 100));
        float xOffset = std::sin(time * 0.8f + i * 0.5f) * 15.f;
        float yOffset = std::cos(time * 1.2f + i * 0.5f) * 15.f;
        corner.setPosition(cornerPositions[i].x - corner.getRadius() + xOffset,
                           cornerPositions[i].y - corner.getRadius() + yOffset);
        window.draw(corner);
    }

    // Draw table pattern
    const int NUM_RINGS = 12;
    for (int i = 0; i < NUM_RINGS; ++i)
    {
        float radius = 80.f + i * 25.f;
        float rotation = time * 20.f + i * 5.f; // Animated rotation

        sf::CircleShape ring(radius, 64);
        ring.setFillColor(sf::Color(0, 0, 0, 0));
        ring.setOutlineThickness(2.f);

        // Alternate between gold and green rings
        if (i % 2 == 0)
        {
            ring.setOutlineColor(sf::Color(255, 215, 0, 40)); // Gold with low alpha
        }
        else
        {
            ring.setOutlineColor(sf::Color(0, 150 - i * 8, 0, 60));
        }

        ring.setOrigin(radius, radius);
        ring.setPosition(centerX, centerY);
        ring.rotate(rotation);
        window.draw(ring);

        // Add inner pattern
        sf::CircleShape innerPattern(radius * 0.8f, 8); // Octagon shape
        innerPattern.setFillColor(sf::Color(0, 0, 0, 0));
        innerPattern.setOutlineThickness(1.f);
        innerPattern.setOutlineColor(sf::Color(0, 130 - i * 8, 0, 40));
        innerPattern.setOrigin(radius * 0.8f, radius * 0.8f);
        innerPattern.setPosition(centerX, centerY);
        innerPattern.rotate(-rotation * 0.5f); // Counter-rotation
        window.draw(innerPattern);
    }

    // Draw players in a circle
    const float playerRadius = 250.f;
    const float angleStep = 360.f / players.size();

    for (size_t i = 0; i < players.size(); ++i)
    {
        Player &p = players[i];
        float angle = (angleStep * i - 90.f) * 3.14159f / 180.f;
        float px = centerX + playerRadius * std::cos(angle);
        float py = centerY + playerRadius * std::sin(angle);

        // Draw player name
        sf::Text nameText;
        nameText.setFont(font);
        nameText.setCharacterSize(24);
        nameText.setString(p.getName() + (p.isBusted() ? " (BUSTED!)" : ""));

        if ((int)i == currentPlayerIndex && !p.isBusted())
        {
            float intensity = (std::sin(time * 4.0f) * 0.3f + 0.7f) * 255;       // Pulsing effect
            nameText.setFillColor(sf::Color(255, 255, 0, (sf::Uint8)intensity)); // Yellow pulsing
        }
        else
        {
            nameText.setFillColor(p.isBusted() ? sf::Color::Red : sf::Color::White);
        }

        sf::FloatRect bounds = nameText.getLocalBounds();
        nameText.setOrigin(bounds.width / 2.f, bounds.height);
        nameText.setPosition(px, py - 80.f);
        window.draw(nameText);

        // Draw player's cards
        const float cardWidth = 75.f;
        const float cardHeight = 100.f;
        const float cardSpacing = 30.f;

        const auto &hand = p.getHand();
        float startX = px - ((cardWidth + cardSpacing) * (hand.size() - 1)) / 2.f;

        for (size_t j = 0; j < hand.size(); ++j)
        {
            float cardX = startX + j * (cardWidth + cardSpacing);
            const auto &card = hand[j];
            card->setPosition(sf::Vector2f(cardX, py - cardHeight / 2.f));
            card->draw(window, font);
        }

        // Draw wildcards if any
        if (p.hasWildcards())
        {
            sf::Text wcText;
            wcText.setFont(font);
            wcText.setCharacterSize(18);
            wcText.setFillColor(sf::Color(255, 215, 0)); // Gold color for wildcards

            std::string wcString = "WC: ";
            for (const auto &w : p.getWildcards())
            {
                wcString += w->getName() + " ";
            }

            wcText.setString(wcString);
            bounds = wcText.getLocalBounds();
            wcText.setOrigin(bounds.width / 2.f, 0.f);
            wcText.setPosition(px, py + 70.f);
            window.draw(wcText);
        }
    }

    // Draw UI elements with subtle glow effects
    hitButton.draw(window);
    standButton.draw(window);
    doubleDownButton.draw(window);
    wildcardButton.draw(window);
    addBetButton.draw(window);

    // Draw info texts with drop shadows
    auto drawTextWithShadow = [&](const sf::Text &text)
    {
        sf::Text shadow = text;
        shadow.setFillColor(sf::Color(0, 0, 0, 100));
        shadow.setPosition(text.getPosition() + sf::Vector2f(2, 2));
        window.draw(shadow);
        window.draw(text);
    };

    drawTextWithShadow(roundInfoText);
    drawTextWithShadow(wildcardInfoText);
    drawTextWithShadow(messageText);
    drawTextWithShadow(potText);

    window.display();
}

const Card &GameState::peekNextCard() const
{
    return deck.peek();
}