#include "states/GameState.hpp"
#include <iostream>
#include <cmath>
#include <algorithm>

GameState::GameState(sf::RenderWindow &window, const GameSettings &settings, const std::vector<PlayerBet> &initialBets)
    : State(window), settings(settings), initialBets(initialBets), currentPlayerIndex(0), roundInProgress(false), roundConcluded(false), waitingForReplay(false), hitButton(sf::Vector2f(0.f, 0.f), sf::Vector2f(150.f, 50.f), "HIT", sf::Font()), standButton(sf::Vector2f(0.f, 0.f), sf::Vector2f(150.f, 50.f), "STAND", sf::Font()), doubleDownButton(sf::Vector2f(0.f, 0.f), sf::Vector2f(200.f, 50.f), "DOUBLE DOWN", sf::Font()), wildcardButton(sf::Vector2f(0.f, 0.f), sf::Vector2f(200.f, 50.f), "WILDCARD", sf::Font())
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

    // Initialize players with bet information
    players.reserve(initialBets.size());
    for (const auto &bet : initialBets)
    {
        players.emplace_back(bet.playerName, bet.startingMoney - bet.currentBet);
    }

    // Button layout configuration
    const float buttonY = window.getSize().y - 70.f;
    const float buttonSpacing = 60.f; // Increased spacing between buttons
    const float regularWidth = 150.f; // Width for Hit and Stand
    const float specialWidth = 280.f; // Increased width for Double Down and Wildcard

    // Calculate total width based on whether wildcard is enabled
    float totalWidth;
    if (settings.wildcardEnabled)
    {
        totalWidth = (regularWidth * 2) + (specialWidth * 2) + (3 * buttonSpacing);
    }
    else
    {
        totalWidth = (regularWidth * 2) + specialWidth + (2 * buttonSpacing);
    }

    // Center the entire button group
    float startX = (window.getSize().x - totalWidth) / 2.f;
    float currentX = startX;

    // Hit button
    hitButton = Button(
        sf::Vector2f(currentX, buttonY),
        sf::Vector2f(regularWidth, 50.f),
        "HIT",
        font,
        sf::Color::White,
        sf::Color::Yellow);
    currentX += regularWidth + buttonSpacing;

    // Stand button
    standButton = Button(
        sf::Vector2f(currentX, buttonY),
        sf::Vector2f(regularWidth, 50.f),
        "STAND",
        font,
        sf::Color::White,
        sf::Color::Yellow);
    currentX += regularWidth + buttonSpacing;

    // Double Down button
    doubleDownButton = Button(
        sf::Vector2f(currentX, buttonY),
        sf::Vector2f(specialWidth, 50.f),
        "DOUBLE DOWN",
        font,
        sf::Color::White,
        sf::Color::Yellow);

    if (settings.wildcardEnabled)
    {
        currentX += specialWidth + buttonSpacing;
        wildcardButton = Button(
            sf::Vector2f(currentX, buttonY),
            sf::Vector2f(specialWidth, 50.f),
            "WILDCARD",
            font,
            sf::Color::White,
            sf::Color::Yellow);
    }

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

    playerMoneyText.setFont(font);
    playerMoneyText.setCharacterSize(24);
    playerMoneyText.setFillColor(sf::Color::Green);
    playerMoneyText.setPosition(50.f, 170.f);

    startNewRound();
}

void GameState::startNewRound()
{
    roundConcluded = false;
    waitingForReplay = false;

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
            else if (event.key.code == sf::Keyboard::Escape)
            {
                requestStateChange(StateChange::Settings);
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
                        if (current.hasForesight())
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
                                nextPlayer();
                            }
                        }
                        updateLabels();
                    }
                    else if (standButton.isMouseOver(mousePos))
                    {
                        messageText.setString(current.getName() + " stands.");
                        nextPlayer();
                    }
                    else if (doubleDownButton.isMouseOver(mousePos))
                    {
                        float currentBet = initialBets[currentPlayerIndex].currentBet;

                        // Check if player has enough money for doubling down
                        if (current.canBet(currentBet))
                        { // Using existing canBet method
                            // Use doubleBet method which already exists
                            current.doubleBet(currentBet);
                            // Update the bet amount in initialBets for proper payout calculation
                            initialBets[currentPlayerIndex].currentBet = currentBet * 2;

                            // Draw one more card and force stand
                            Card c = deck.draw();
                            current.addCard(std::make_shared<Card>(c));

                            if (current.isBusted())
                            {
                                messageText.setString(current.getName() + " BUSTED!");
                            }
                            else
                            {
                                messageText.setString(current.getName() + " doubled down!");
                            }
                            nextPlayer();
                            updateLabels();
                        }
                        else
                        {
                            messageText.setString(current.getName() + " doesn't have enough money to double down!");
                        }
                    }
                    else if (settings.wildcardEnabled && wildcardButton.isMouseOver(mousePos))
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
                }
            }
        }
        else if (event.type == sf::Event::MouseMoved)
        {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            hitButton.setHighlight(hitButton.isMouseOver(mousePos));
            standButton.setHighlight(standButton.isMouseOver(mousePos));
            doubleDownButton.setHighlight(doubleDownButton.isMouseOver(mousePos));
            if (settings.wildcardEnabled)
            {
                wildcardButton.setHighlight(wildcardButton.isMouseOver(mousePos));
            }
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

    animationTime += 1.f / 60.f;

    // Update decorative circles
    const float radius = 200.f;
    for (size_t i = 0; i < cornerCircles.size(); ++i)
    {
        float xOffset = std::sin(animationTime * CIRCLE_ANIMATION_SPEED + i * 0.5f) * 15.f;
        float yOffset = std::cos(animationTime * CIRCLE_ANIMATION_SPEED + i * 0.5f) * 15.f;

        float x = (i % 2) ? window.getSize().x - radius : -radius + xOffset;
        float y = (i / 2) ? window.getSize().y - radius : -radius + yOffset;

        cornerCircles[i].setPosition(x, y);
    }

    // Update UI elements
    float dt = 1.f / 60.f;
    hitButton.update(dt);
    standButton.update(dt);
    doubleDownButton.update(dt);
    if (settings.wildcardEnabled)
    {
        wildcardButton.update(dt);
    }
}

void GameState::nextPlayer()
{
    currentPlayerIndex++;
    if (currentPlayerIndex >= static_cast<int>(players.size()))
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
    for (int i = 0; i < static_cast<int>(players.size()); ++i)
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
        messageText.setString("Everyone busted! Press R to replay or ESC for settings.");
    }
    else
    {
        std::string winnerText = "Winner(s): ";
        for (size_t i = 0; i < winners.size(); ++i)
        {
            Player &winner = players[winners[i]];
            float betAmount = initialBets[winners[i]].currentBet;
            float winnings = betAmount * 2; // 1:1 payout on the total bet (including doubled bets)

            winner.addWinnings(winnings);

            winnerText += winner.getName() + " ($" + std::to_string(static_cast<int>(winnings)) + ")";
            if (i < winners.size() - 1)
            {
                winnerText += ", ";
            }
        }
        winnerText += " with " + std::to_string(bestScore) + "! Press R to replay or ESC for settings.";
        messageText.setString(winnerText);
    }

    waitingForReplay = true;
    updateLabels();
}

void GameState::updateLabels()
{
    if (currentPlayerIndex < static_cast<int>(players.size()))
    {
        Player &current = players[currentPlayerIndex];
        roundInfoText.setString(current.getName() + " - Total: " +
                                std::to_string(current.calculateHandTotal()));

        playerMoneyText.setString("Money: $" + std::to_string(static_cast<int>(current.getMoney())));

        if (settings.wildcardEnabled)
        {
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
    }
    else
    {
        roundInfoText.setString("Round Complete");
        wildcardInfoText.setString("");
        playerMoneyText.setString("");
    }
}

void GameState::render()
{
    window.clear(sf::Color(0, 60, 0));

    // Draw decorative corner circles
    for (const auto &circle : cornerCircles)
    {
        window.draw(circle);
    }

    // Draw players in a circle
    const float centerX = window.getSize().x / 2.f;
    const float centerY = window.getSize().y / 2.f;
    const float playerRadius = 250.f;
    const float angleStep = 360.f / players.size();

    for (size_t i = 0; i < players.size(); ++i)
    {
        Player &p = players[i];
        float angle = (angleStep * i - 90.f) * 3.14159f / 180.f;
        float px = centerX + playerRadius * std::cos(angle);
        float py = centerY + playerRadius * std::sin(angle);

        // Draw player info
        sf::Text nameText;
        nameText.setFont(font);
        nameText.setCharacterSize(24);
        sf::String displayText = p.getName();
        if (p.isBusted())
        {
            displayText += " (BUSTED!)";
        }
        if (static_cast<int>(i) == currentPlayerIndex && !roundConcluded)
        {
            float intensity = (std::sin(animationTime * 4.0f) * 0.3f + 0.7f) * 255;
            nameText.setFillColor(sf::Color(255, 255, 0, (sf::Uint8)intensity));
        }
        else
        {
            nameText.setFillColor(p.isBusted() ? sf::Color::Red : sf::Color::White);
        }

        // Center name text
        sf::FloatRect bounds = nameText.getLocalBounds();
        nameText.setOrigin(bounds.width / 2.f, bounds.height);
        nameText.setPosition(px, py - 80.f);
        window.draw(nameText);

        // Draw bet amount
        sf::Text betText;
        betText.setFont(font);
        betText.setCharacterSize(20);
        betText.setString("Bet: $" + std::to_string(static_cast<int>(initialBets[i].currentBet)));
        betText.setFillColor(sf::Color(255, 215, 0)); // Gold color
        bounds = betText.getLocalBounds();
        betText.setOrigin(bounds.width / 2.f, bounds.height);
        betText.setPosition(px, py - 55.f);
        window.draw(betText);

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

            // Only show clear cards for active player
            if (static_cast<int>(i) == currentPlayerIndex)
            {
                card->setPosition(sf::Vector2f(cardX, py - cardHeight / 2.f));
                card->draw(window, font);
            }
            else
            {
                // Draw blurred/hidden card back for non-active players
                sf::RectangleShape cardBack(sf::Vector2f(cardWidth, cardHeight));
                cardBack.setPosition(cardX, py - cardHeight / 2.f);
                cardBack.setFillColor(sf::Color(30, 30, 30, 200));
                cardBack.setOutlineColor(sf::Color(100, 100, 100, 100));
                cardBack.setOutlineThickness(2.f);

                // Add blur effect pattern
                sf::RectangleShape pattern(sf::Vector2f(cardWidth, cardHeight));
                pattern.setPosition(cardX, py - cardHeight / 2.f);
                pattern.setFillColor(sf::Color(50, 50, 50, 50));

                window.draw(cardBack);
                window.draw(pattern);

                // Add a "?" symbol
                sf::Text hiddenSymbol;
                hiddenSymbol.setFont(font);
                hiddenSymbol.setString("?");
                hiddenSymbol.setCharacterSize(32);
                hiddenSymbol.setFillColor(sf::Color(150, 150, 150, 150));

                bounds = hiddenSymbol.getLocalBounds();
                hiddenSymbol.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
                hiddenSymbol.setPosition(cardX + cardWidth / 2.f, py);

                window.draw(hiddenSymbol);
            }
        }

        // Draw wildcards if any (only for active player)
        if (settings.wildcardEnabled && static_cast<int>(i) == currentPlayerIndex && p.hasWildcards())
        {
            sf::Text wcText;
            wcText.setFont(font);
            wcText.setCharacterSize(18);
            wcText.setFillColor(sf::Color(255, 215, 0));

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
    if (settings.wildcardEnabled)
    {
        wildcardButton.draw(window);
    }

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
    drawTextWithShadow(playerMoneyText);

    window.display();
}

const Card &GameState::peekNextCard() const
{
    return deck.peek();
}