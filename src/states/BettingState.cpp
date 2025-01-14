#include "BettingState.hpp"
#include <iostream>
#include <cmath>
#include <algorithm>

BettingState::BettingState(sf::RenderWindow& window, const GameSettings& settings)
    : State(window)
    , settings(settings)
    , currentPlayerIndex(0)
    , currentBetAmount(settings.minBet)
    , increaseBetButton(sf::Vector2f(0, 0), sf::Vector2f(60, 50), "+", sf::Font(), sf::Color::White, sf::Color::Yellow)
    , decreaseBetButton(sf::Vector2f(0, 0), sf::Vector2f(60, 50), "-", sf::Font(), sf::Color::White, sf::Color::Yellow)
    , confirmBetButton(sf::Vector2f(0, 0), sf::Vector2f(200, 50), "CONFIRM BET", sf::Font(), sf::Color::White, sf::Color::Yellow)
    , backButton(sf::Vector2f(0, 0), sf::Vector2f(120, 50), "BACK", sf::Font(), sf::Color::White, sf::Color::Yellow)
{
    if (!loadResources()) {
        window.close();
        return;
    }

    // Initialize player bets
    playerBets.reserve(settings.numPlayers);
    for (int i = 0; i < settings.numPlayers; ++i) {
        PlayerBet bet{
            "Player " + std::to_string(i + 1),
            settings.startingMoney,
            0.0f,
            false
        };
        playerBets.push_back(bet);
    }

    initializeDecorations();
    initializeButtons();
    updateLabels();
}

bool BettingState::loadResources() {
    // Load casino-style title font
    if (!titleFont.loadFromFile("src/assets/font/Casino.ttf")) {
        std::cerr << "Error: Could not load title font\n";
        return false;
    }

    // Load button font
    if (!buttonFont.loadFromFile("src/assets/font/Troska.ttf")) {
        std::cerr << "Error: Could not load button font file.\n";
        return false;
    }

    // Load standard font with fallbacks
    std::vector<std::string> fontPaths = {
        "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf",
        "/Library/Fonts/Verdana.ttf",
        "C:/Windows/Fonts/verdana.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "arial.ttf",
        "C:/Windows/Fonts/arial.ttf",
        "/System/Library/Fonts/Helvetica.ttf"
    };
    
    bool standardFontLoaded = false;
    for (const auto& path : fontPaths) {
        if (standardFont.loadFromFile(path)) {
            standardFontLoaded = true;
            break;
        }
    }
    
    if (!standardFontLoaded) {
        std::cerr << "Error: Could not load standard font\n";
        return false;
    }

    // Initialize title text
    titleText.setFont(titleFont);
    titleText.setString("Place Your Bets");
    titleText.setCharacterSize(80);
    titleText.setLetterSpacing(2.0f);
    titleText.setFillColor(sf::Color(255, 215, 0));  // Gold color
    titleText.setOutlineThickness(3);
    titleText.setOutlineColor(sf::Color(139, 69, 19));
    
    // Center the title
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setOrigin(titleBounds.width / 2, titleBounds.height / 2);
    titleText.setPosition(window.getSize().x / 2.f, window.getSize().y / 4.f);

    // Initialize other text elements
    currentPlayerText.setFont(standardFont);
    betAmountText.setFont(standardFont);
    instructionText.setFont(standardFont);

    currentPlayerText.setCharacterSize(32);
    betAmountText.setCharacterSize(48);
    instructionText.setCharacterSize(24);

    currentPlayerText.setFillColor(sf::Color::White);
    betAmountText.setFillColor(sf::Color(255, 215, 0));  // Gold
    instructionText.setFillColor(sf::Color(220, 220, 220));

    return true;
}

void BettingState::initializeDecorations() {
    // Create decorative circles similar to MenuState
    CircleData circle1;
    circle1.shape.setRadius(200.f);
    circle1.basePos = sf::Vector2f(-100.f, -100.f);
    circle1.xFreq = 0.8f;
    circle1.yFreq = 1.5f;
    circle1.xAmp = 15.0f;
    circle1.yAmp = 20.0f;
    circle1.phase = 0.0f;
    circle1.shape.setFillColor(sf::Color(0, 70, 0));

    CircleData circle2;
    circle2.shape.setRadius(150.f);
    circle2.basePos = sf::Vector2f(window.getSize().x - 100.f, window.getSize().y - 100.f);
    circle2.xFreq = 0.9f;
    circle2.yFreq = 1.2f;
    circle2.xAmp = 20.0f;
    circle2.yAmp = 25.0f;
    circle2.phase = 0.5f;
    circle2.shape.setFillColor(sf::Color(0, 70, 0));

    decorativeCircles.push_back(circle1);
    decorativeCircles.push_back(circle2);
}

void BettingState::initializeButtons() {
    float centerX = window.getSize().x / 2.f;
    float centerY = window.getSize().y / 2.f;

    const float adjustButtonWidth = 60.f;
    const float adjustButtonHeight = 50.f;

    // Position decrease button with minus symbol
    decreaseBetButton = Button(
        sf::Vector2f(centerX - 150.f, centerY),
        sf::Vector2f(adjustButtonWidth, adjustButtonHeight),
        "",
        buttonFont,
        sf::Color(220, 220, 220),
        sf::Color(255, 215, 0)
    );
    
    sf::RectangleShape minusShape;
    minusShape.setSize(sf::Vector2f(20.f, 4.f));
    minusShape.setFillColor(sf::Color::White);
    minusShape.setOrigin(minusShape.getSize().x / 2.f, minusShape.getSize().y / 2.f);
    minusShape.setPosition(
        decreaseBetButton.getPosition().x + adjustButtonWidth / 2.f,
        decreaseBetButton.getPosition().y + adjustButtonHeight / 2.f
    );
    minusSymbols.push_back(minusShape);

    // Position increase button with plus symbol
    increaseBetButton = Button(
        sf::Vector2f(centerX + 90.f, centerY),
        sf::Vector2f(adjustButtonWidth, adjustButtonHeight),
        "",
        buttonFont,
        sf::Color(220, 220, 220),
        sf::Color(255, 215, 0)
    );

    sf::RectangleShape plusVertical;
    plusVertical.setSize(sf::Vector2f(4.f, 20.f));
    plusVertical.setFillColor(sf::Color::White);
    plusVertical.setOrigin(plusVertical.getSize().x / 2.f, plusVertical.getSize().y / 2.f);
    plusVertical.setPosition(
        increaseBetButton.getPosition().x + adjustButtonWidth / 2.f,
        increaseBetButton.getPosition().y + adjustButtonHeight / 2.f
    );

    sf::RectangleShape plusHorizontal;
    plusHorizontal.setSize(sf::Vector2f(20.f, 4.f));
    plusHorizontal.setFillColor(sf::Color::White);
    plusHorizontal.setOrigin(plusHorizontal.getSize().x / 2.f, plusHorizontal.getSize().y / 2.f);
    plusHorizontal.setPosition(
        increaseBetButton.getPosition().x + adjustButtonWidth / 2.f,
        increaseBetButton.getPosition().y + adjustButtonHeight / 2.f
    );

    plusVerticalSymbols.push_back(plusVertical);
    plusHorizontalSymbols.push_back(plusHorizontal);

    // Confirm bet button below bet controls
    confirmBetButton = Button(
        sf::Vector2f(centerX - 100.f, centerY + 100.f),
        sf::Vector2f(200.f, 50.f),
        "CONFIRM BET",
        buttonFont,
        sf::Color(220, 220, 220),
        sf::Color(255, 215, 0)
    );

    // Back button in bottom left
    backButton = Button(
        sf::Vector2f(50.f, window.getSize().y - 70.f),
        sf::Vector2f(120.f, 50.f),
        "BACK",
        buttonFont,
        sf::Color(220, 220, 220),
        sf::Color(255, 215, 0)
    );
}

void BettingState::validateBetAmount() {
    float minBet = settings.minBet * MIN_BET_MULTIPLIER;
    float maxBet = settings.minBet * MAX_BET_MULTIPLIER;
    float playerMoney = playerBets[currentPlayerIndex].startingMoney;
    
    currentBetAmount = std::clamp(currentBetAmount, minBet, 
                                 std::min(maxBet, playerMoney));
}

void BettingState::updateLabels() {
    PlayerBet& current = playerBets[currentPlayerIndex];
    
    currentPlayerText.setString(current.playerName + "'s Turn");
    sf::FloatRect bounds = currentPlayerText.getLocalBounds();
    currentPlayerText.setOrigin(bounds.width / 2, bounds.height / 2);
    currentPlayerText.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f - 100.f);

    betAmountText.setString("$" + std::to_string(static_cast<int>(currentBetAmount)));
    bounds = betAmountText.getLocalBounds();
    betAmountText.setOrigin(bounds.width / 2, bounds.height / 2);
    betAmountText.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);

    std::string instruction = "Available Money: $" + 
                            std::to_string(static_cast<int>(current.startingMoney));
    instructionText.setString(instruction);
    bounds = instructionText.getLocalBounds();
    instructionText.setOrigin(bounds.width / 2, bounds.height / 2);
    instructionText.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f - 50.f);
}

void BettingState::nextPlayer() {
    if (currentPlayerIndex + 1 < playerBets.size()) {
        currentPlayerIndex++;
        currentBetAmount = settings.minBet;
        updateLabels();
    } else {
        // All players have bet, transition to game state
        requestStateChange(StateChange::Game);
    }
}

void BettingState::handleInput() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        
        if (event.type == sf::Event::MouseButtonPressed && 
            event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos = getMousePosition();
            
            if (increaseBetButton.isMouseOver(mousePos)) {
                currentBetAmount += BET_INCREMENT;
                validateBetAmount();
                updateLabels();
            }
            else if (decreaseBetButton.isMouseOver(mousePos)) {
                currentBetAmount -= BET_INCREMENT;
                validateBetAmount();
                updateLabels();
            }
            else if (confirmBetButton.isMouseOver(mousePos)) {
                PlayerBet& current = playerBets[currentPlayerIndex];
                current.currentBet = currentBetAmount;
                current.startingMoney -= currentBetAmount;
                current.hasBet = true;
                nextPlayer();
            }
            else if (backButton.isMouseOver(mousePos)) {
                requestStateChange(StateChange::Settings);
            }
        }
        
        if (event.type == sf::Event::MouseMoved) {
            sf::Vector2f mousePos = getMousePosition();
            increaseBetButton.setHighlight(increaseBetButton.isMouseOver(mousePos));
            decreaseBetButton.setHighlight(decreaseBetButton.isMouseOver(mousePos));
            confirmBetButton.setHighlight(confirmBetButton.isMouseOver(mousePos));
            backButton.setHighlight(backButton.isMouseOver(mousePos));
        }
    }
}

void BettingState::update() {
    static float time = 0.0f;
    const float deltaTime = 0.016f;
    time += deltaTime;

    // Animate title
    float titleOffset = std::sin(time * 2.f) * 5.f;
    titleText.setPosition(window.getSize().x / 2.f, window.getSize().y / 4.f + titleOffset);

    // Update decorative circles
    for (auto& circle : decorativeCircles) {
        float xOffset = std::sin(time * circle.xFreq + circle.phase) * circle.xAmp;
        float yOffset = std::cos(time * circle.yFreq + circle.phase) * circle.yAmp;
        float circularX = std::cos(time * 0.5f + circle.phase) * (circle.xAmp * 0.5f);
        float circularY = std::sin(time * 0.5f + circle.phase) * (circle.yAmp * 0.5f);
        
        circle.shape.setPosition(
            circle.basePos.x + xOffset + circularX,
            circle.basePos.y + yOffset + circularY
        );
    }

    // Update buttons
    increaseBetButton.update(deltaTime);
    decreaseBetButton.update(deltaTime);
    confirmBetButton.update(deltaTime);
    backButton.update(deltaTime);
}

void BettingState::render() {
    window.clear(sf::Color(0, 60, 0));

    // Draw decorative circles
    for (const auto& circle : decorativeCircles) {
        window.draw(circle.shape);
    }

    // Draw title with shadow
    sf::Text shadowText = titleText;
    shadowText.setFillColor(sf::Color(0, 40, 0));
    shadowText.setPosition(titleText.getPosition() + sf::Vector2f(4, 4));
    window.draw(shadowText);
    window.draw(titleText);

    // Draw main UI elements
    window.draw(currentPlayerText);
    window.draw(betAmountText);
    window.draw(instructionText);

    // Draw buttons
    increaseBetButton.draw(window);
    decreaseBetButton.draw(window);
    confirmBetButton.draw(window);
    backButton.draw(window);

    // Draw plus/minus symbols
    window.draw(minusSymbols[0]);
    window.draw(plusVerticalSymbols[0]);
    window.draw(plusHorizontalSymbols[0]);

    window.display();
}

sf::Vector2f BettingState::getMousePosition() const {
    return window.mapPixelToCoords(sf::Mouse::getPosition(window));
}