#include "GameState.hpp"
#include <random>
#include <algorithm>
#include <sstream>
#include <iomanip>

Card::Card(int value) : value(value) {
    shape.setSize(sf::Vector2f(75.f, 100.f));
    shape.setFillColor(sf::Color::White);
    shape.setOutlineThickness(2);
    shape.setOutlineColor(sf::Color::Black);
}

void Card::setPosition(const sf::Vector2f& pos) {
    shape.setPosition(pos);
    text.setPosition(pos.x + 10, pos.y + 10);
}

void Card::draw(sf::RenderWindow& window, const sf::Font& font, bool faceUp) {
    window.draw(shape);
    if (faceUp) {
        text.setFont(font);
        text.setString(std::to_string(value));
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::Black);
        window.draw(text);
    }
}

GameState::GameState(sf::RenderWindow& window) 
    : State(window), 
      playerTurn(true), 
      gameOver(false), 
      dealerRevealed(false),
      bettingPhase(true),
      playerMoney(500.0f),
      currentBet(0.0f),
      minBet(10.0f) {
    loadResources();
    initializeButtons();
    updateMoneyText();
}

void GameState::loadResources() {
    std::vector<std::string> fontPaths = {
        "arial.ttf",
        "C:/Windows/Fonts/arial.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "/System/Library/Fonts/Helvetica.ttf"
    };
    
    bool fontLoaded = false;
    for (const auto& path : fontPaths) {
        if (font.loadFromFile(path)) {
            fontLoaded = true;
            break;
        }
    }
    
    if (!fontLoaded) {
        std::cerr << "Error: Could not load font file." << std::endl;
        return;
    }

    // Setup texts
    playerScoreText.setFont(font);
    playerScoreText.setCharacterSize(24);
    playerScoreText.setFillColor(sf::Color::White);
    playerScoreText.setPosition(10, 500);

    dealerScoreText.setFont(font);
    dealerScoreText.setCharacterSize(24);
    dealerScoreText.setFillColor(sf::Color::White);
    dealerScoreText.setPosition(10, 50);

    messageText.setFont(font);
    messageText.setCharacterSize(32);
    messageText.setFillColor(sf::Color::White);
    messageText.setPosition(300, 250);

    moneyText.setFont(font);
    moneyText.setCharacterSize(24);
    moneyText.setFillColor(sf::Color::White);
    moneyText.setPosition(10, 10);

    betText.setFont(font);
    betText.setCharacterSize(24);
    betText.setFillColor(sf::Color::White);
    betText.setPosition(10, 550);
}

void GameState::initializeButtons() {
    // Game buttons
    buttons.emplace_back(
        sf::Vector2f(600.f, 500.f),
        sf::Vector2f(100.f, 40.f),
        "Hit",
        font
    );

    buttons.emplace_back(
        sf::Vector2f(600.f, 550.f),
        sf::Vector2f(100.f, 40.f),
        "Stand",
        font
    );

    // Betting buttons
    buttons.emplace_back(
        sf::Vector2f(300.f, 300.f),
        sf::Vector2f(150.f, 40.f),
        "Bet $10",
        font
    );

    buttons.emplace_back(
        sf::Vector2f(300.f, 350.f),
        sf::Vector2f(150.f, 40.f),
        "Deal",
        font
    );
}

void GameState::handleInput() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (event.type == sf::Event::MouseButtonPressed && 
            event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos = getMousePosition();

            if (bettingPhase) {
                // Bet button
                if (buttons[2].isMouseOver(mousePos) && playerMoney >= minBet) {
                    currentBet += minBet;
                    playerMoney -= minBet;
                    updateMoneyText();
                }
                // Deal button
                else if (buttons[3].isMouseOver(mousePos) && currentBet > 0) {
                    bettingPhase = false;
                    dealInitialCards();
                }
            }
            else if (!gameOver && playerTurn) {
                // Hit button
                if (buttons[0].isMouseOver(mousePos)) {
                    std::random_device rd;
                    std::mt19937 gen(rd());
                    std::uniform_int_distribution<> dis(1, 11);
                    
                    auto newCard = std::make_shared<Card>(dis(gen));
                    newCard->setPosition(sf::Vector2f(300.f + playerHand.size() * 85.f, 400.f));
                    playerHand.push_back(newCard);

                    if (calculateHandTotal(playerHand) > 21) {
                        gameOver = true;
                        dealerRevealed = true;
                        handleLoss();
                    }
                }
                // Stand button
                else if (buttons[1].isMouseOver(mousePos)) {
                    playerTurn = false;
                    dealerRevealed = true;
                    dealerPlay();
                }
            }
        }

        // Mouse move for button highlights
        if (event.type == sf::Event::MouseMoved) {
            sf::Vector2f mousePos = getMousePosition();
            for (auto& button : buttons) {
                button.setHighlight(button.isMouseOver(mousePos));
            }
        }

        // Handle restart
        if (gameOver && event.type == sf::Event::KeyPressed && 
            event.key.code == sf::Keyboard::R) {
            if (playerMoney >= minBet) {
                startNewBet();
            } else {
                // Game over, back to menu
                requestStateChange(StateChange::Menu);
            }
        }
    }
}

void GameState::dealInitialCards() {
    playerHand.clear();
    dealerHand.clear();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 11);

    for (int i = 0; i < 2; ++i) {
        auto playerCard = std::make_shared<Card>(dis(gen));
        auto dealerCard = std::make_shared<Card>(dis(gen));

        playerCard->setPosition(sf::Vector2f(300.f + i * 85.f, 400.f));
        dealerCard->setPosition(sf::Vector2f(300.f + i * 85.f, 100.f));

        playerHand.push_back(playerCard);
        dealerHand.push_back(dealerCard);
    }

    playerTurn = true;
    gameOver = false;
    dealerRevealed = false;
}

void GameState::dealerPlay() {
    while (calculateHandTotal(dealerHand) < 17) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 11);
        
        auto newCard = std::make_shared<Card>(dis(gen));
        newCard->setPosition(sf::Vector2f(300.f + dealerHand.size() * 85.f, 100.f));
        dealerHand.push_back(newCard);
    }
    checkGameOver();
}

void GameState::checkGameOver() {
    int playerTotal = calculateHandTotal(playerHand);
    int dealerTotal = calculateHandTotal(dealerHand);

    gameOver = true;

    if (dealerTotal > 21) {
        handleWin();
    } else if (dealerTotal > playerTotal) {
        handleLoss();
    } else if (playerTotal > dealerTotal) {
        handleWin();
    } else {
        handlePush();
    }
}

void GameState::handleWin() {
    playerMoney += currentBet * 2;  // Return bet + winnings
    messageText.setString("You win! (R to play again)");
    updateMoneyText();
}

void GameState::handleLoss() {
    messageText.setString("Dealer wins! (R to play again)");
    updateMoneyText();
}

void GameState::handlePush() {
    playerMoney += currentBet;  // Return bet
    messageText.setString("Push! (R to play again)");
    updateMoneyText();
}

void GameState::startNewBet() {
    bettingPhase = true;
    currentBet = 0;
    updateMoneyText();
    messageText.setString("");
}

void GameState::updateMoneyText() {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2);
    ss << "Money: $" << playerMoney;
    moneyText.setString(ss.str());
    
    ss.str("");
    ss << "Current Bet: $" << currentBet;
    betText.setString(ss.str());
}

int GameState::calculateHandTotal(const std::vector<std::shared_ptr<Card>>& hand) {
    int total = 0;
    for (const auto& card : hand) {
        total += card->getValue();
    }
    return total;
}

void GameState::update() {
    playerScoreText.setString("Player Score: " + 
        std::to_string(calculateHandTotal(playerHand)));
    
    if (dealerRevealed) {
        dealerScoreText.setString("Dealer Score: " + 
            std::to_string(calculateHandTotal(dealerHand)));
    } else {
        dealerScoreText.setString("Dealer Score: ?");
    }
}

void GameState::render() {
    window.clear(sf::Color(0, 100, 0));  // Dark green background

    // Draw money info
    window.draw(moneyText);
    window.draw(betText);

    if (bettingPhase) {
        // Draw betting buttons
        buttons[2].draw(window);  // Bet button
        buttons[3].draw(window);  // Deal button
    } else {
        // Draw cards
        for (const auto& card : playerHand) {
            card->draw(window, font);
        }
        
        for (size_t i = 0; i < dealerHand.size(); ++i) {
            dealerHand[i]->draw(window, font, dealerRevealed || i == 0);
        }

        // Draw game buttons
        buttons[0].draw(window);  // Hit button
        buttons[1].draw(window);  // Stand button

        // Draw scores and messages
        window.draw(playerScoreText);
        window.draw(dealerScoreText);
    }

    if (gameOver || bettingPhase) {
        window.draw(messageText);
    }

    window.display();
}

sf::Vector2f GameState::getMousePosition() const {
    return window.mapPixelToCoords(sf::Mouse::getPosition(window));
}