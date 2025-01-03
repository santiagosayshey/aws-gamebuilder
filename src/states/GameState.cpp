#include "GameState.hpp"
#include <random>
#include <algorithm>
#include <sstream>
#include <iomanip>

GameState::GameState(sf::RenderWindow& window, const GameSettings& settings)
    : State(window)
    , currentPlayerIndex(0)
    , dealer("Dealer", 0.0f)
    , playerTurn(true)
    , gameOver(false)
    , dealerRevealed(false)
    , bettingPhase(true)
    , minBet(settings.minBet) 
{
    // Initialize players
    for (int i = 0; i < settings.numPlayers; ++i) {
        players.emplace_back("Player " + std::to_string(i + 1), settings.startingMoney);
    }
    
    if (!loadResources()) {
        window.close();
        return;
    }
    
    initializeButtons();
    updateMoneyText();
}

bool GameState::loadResources() {
    std::vector<std::string> fontPaths = {
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "arial.ttf",
        "C:/Windows/Fonts/arial.ttf",
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
        return false;
    }

    // Initialize text elements for each player
    float yOffset = 400.f;
    for (size_t i = 0; i < players.size(); ++i) {
        // Score text
        sf::Text scoreText;
        scoreText.setFont(font);
        scoreText.setCharacterSize(24);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(10.f, yOffset + i * 50.f);
        playerScoreTexts.push_back(scoreText);

        // Money text
        sf::Text moneyText;
        moneyText.setFont(font);
        moneyText.setCharacterSize(24);
        moneyText.setFillColor(sf::Color::White);
        moneyText.setPosition(200.f, yOffset + i * 50.f);
        playerMoneyTexts.push_back(moneyText);

        // Bet text
        sf::Text betText;
        betText.setFont(font);
        betText.setCharacterSize(24);
        betText.setFillColor(sf::Color::White);
        betText.setPosition(400.f, yOffset + i * 50.f);
        playerBetTexts.push_back(betText);
    }

    // Dealer and message text setup
    dealerScoreText.setFont(font);
    dealerScoreText.setCharacterSize(24);
    dealerScoreText.setFillColor(sf::Color::White);
    dealerScoreText.setPosition(10, 50);

    messageText.setFont(font);
    messageText.setCharacterSize(32);
    messageText.setFillColor(sf::Color::White);
    messageText.setPosition(300, 250);

    currentPlayerText.setFont(font);
    currentPlayerText.setCharacterSize(32);
    currentPlayerText.setFillColor(sf::Color::Yellow);
    currentPlayerText.setPosition(300, 200);

    return true;
}

void GameState::initializeButtons() {
    const std::string buttonTexture = "src/assets/ui/button.png";

    // Game buttons
    buttons.emplace_back(
        sf::Vector2f(600.f, 500.f), 
        sf::Vector2f(100.f, 40.f), 
        "Hit", 
        font, 
        buttonTexture
    );

    buttons.emplace_back(
        sf::Vector2f(600.f, 550.f), 
        sf::Vector2f(100.f, 40.f), 
        "Stand", 
        font, 
        buttonTexture
    );

    // Betting buttons
    buttons.emplace_back(
        sf::Vector2f(300.f, 300.f), 
        sf::Vector2f(150.f, 40.f), 
        "Bet $10", 
        font, 
        buttonTexture
    );

    buttons.emplace_back(
        sf::Vector2f(300.f, 350.f), 
        sf::Vector2f(150.f, 40.f), 
        "Deal", 
        font, 
        buttonTexture
    );
}

sf::Vector2f GameState::getMousePosition() const {
    return window.mapPixelToCoords(sf::Mouse::getPosition(window));
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
                if (buttons[2].isMouseOver(mousePos) && players[currentPlayerIndex].canBet(minBet)) {
                    players[currentPlayerIndex].placeBet(minBet);
                    updateMoneyText();
                }
                // Deal button
                else if (buttons[3].isMouseOver(mousePos) && players[currentPlayerIndex].getCurrentBet() > 0) {
                    if (currentPlayerIndex < players.size() - 1) {
                        // Move to next player's betting turn
                        currentPlayerIndex++;
                        updateMoneyText();
                    } else {
                        // All players have bet, start the game
                        bettingPhase = false;
                        currentPlayerIndex = 0;
                        dealInitialCards();
                    }
                }
            }
            else if (!gameOver && playerTurn) {
                Player& currentPlayer = players[currentPlayerIndex];
                // Hit button
                if (buttons[0].isMouseOver(mousePos)) {
                    std::random_device rd;
                    std::mt19937 gen(rd());
                    std::uniform_int_distribution<> dis(1, 11);
                    
                    auto newCard = std::make_shared<Card>(dis(gen));
                    float xOffset = 300.f + currentPlayer.getHand().size() * 85.f;
                    float yOffset = 400.f + currentPlayerIndex * 50.f;
                    newCard->setPosition(sf::Vector2f(xOffset, yOffset));
                    currentPlayer.addCard(newCard);

                    if (currentPlayer.isBusted()) {
                        handleLoss(currentPlayer);
                        nextPlayer();
                    }
                }
                // Stand button
                else if (buttons[1].isMouseOver(mousePos)) {
                    nextPlayer();
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
            bool anyPlayerCanBet = false;
            for (const auto& player : players) {
                if (player.canBet(minBet)) {
                    anyPlayerCanBet = true;
                    break;
                }
            }
            
            if (anyPlayerCanBet) {
                startNewBet();
            } else {
                requestStateChange(StateChange::Menu);
            }
        }
    }
}

void GameState::nextPlayer() {
    if (currentPlayerIndex < players.size() - 1) {
        currentPlayerIndex++;
    } else {
        playerTurn = false;
        dealerRevealed = true;
        dealerPlay();
    }
}

void GameState::dealInitialCards() {
    // Clear all hands
    for (auto& player : players) {
        player.clearHand();
    }
    dealer.clearHand();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 11);

    // Deal two cards to each player
    for (size_t i = 0; i < players.size(); ++i) {
        for (int j = 0; j < 2; ++j) {
            auto card = std::make_shared<Card>(dis(gen));
            float xOffset = 300.f + j * 85.f;
            float yOffset = 400.f + i * 50.f;
            card->setPosition(sf::Vector2f(xOffset, yOffset));
            players[i].addCard(card);
        }
    }

    // Deal to dealer
    for (int i = 0; i < 2; ++i) {
        auto card = std::make_shared<Card>(dis(gen));
        card->setPosition(sf::Vector2f(300.f + i * 85.f, 100.f));
        dealer.addCard(card);
    }

    playerTurn = true;
    gameOver = false;
    dealerRevealed = false;
    currentPlayerIndex = 0;
}

void GameState::dealerPlay() {
    while (dealer.calculateHandTotal() < 17) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 11);
        
        auto newCard = std::make_shared<Card>(dis(gen));
        newCard->setPosition(sf::Vector2f(300.f + dealer.getHand().size() * 85.f, 100.f));
        dealer.addCard(newCard);
    }
    checkGameOver();
}

void GameState::checkGameOver() {
    gameOver = true;
    dealerRevealed = true;
    int dealerTotal = dealer.calculateHandTotal();
    bool dealerBusted = dealer.isBusted();

    for (auto& player : players) {
        int playerTotal = player.calculateHandTotal();
        
        if (dealerBusted) {
            if (!player.isBusted()) {
                handleWin(player);
            } else {
                handleLoss(player);
            }
        } else if (player.isBusted()) {
            handleLoss(player);
        } else if (dealerTotal > playerTotal) {
            handleLoss(player);
        } else if (playerTotal > dealerTotal) {
            handleWin(player);
        } else {
            handlePush(player);
        }
    }
}

void GameState::handleWin(Player& player) {
    player.addWinnings(player.getCurrentBet() * 2);
    updateMoneyText();
}

void GameState::handleLoss(Player& player) {
    messageText.setString(player.getName() + " loses!");
    updateMoneyText();
}

void GameState::handlePush(Player& player) {
    player.addWinnings(player.getCurrentBet());
    updateMoneyText();
}

void GameState::startNewBet() {
    bettingPhase = true;
    currentPlayerIndex = 0;
    for (auto& player : players) {
        player.clearHand();
    }
    dealer.clearHand();
    updateMoneyText();
    messageText.setString("");
}

void GameState::updateMoneyText() {
    for (size_t i = 0; i < players.size(); ++i) {
        const auto& player = players[i];
        
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2);
        
        // Update money text
        ss << "Money: $" << player.getMoney();
        playerMoneyTexts[i].setString(ss.str());
        
        // Update bet text
        ss.str("");
        ss << "Bet: $" << player.getCurrentBet();
        playerBetTexts[i].setString(ss.str());
    }

    currentPlayerText.setString("Current Turn: Player " + 
        std::to_string(currentPlayerIndex + 1));
}

void GameState::update() {
    // Update player scores
    for (size_t i = 0; i < players.size(); ++i) {
        playerScoreTexts[i].setString("Player " + std::to_string(i + 1) + 
            " Score: " + std::to_string(players[i].calculateHandTotal()));
    }
    
    // Update dealer score
    if (dealerRevealed) {
        dealerScoreText.setString("Dealer Score: " + 
            std::to_string(dealer.calculateHandTotal()));
    } else {
        dealerScoreText.setString("Dealer Score: ?");
    }
}

void GameState::render() {
    window.clear(sf::Color(0, 100, 0));  // Dark green background

    // Draw player info
    for (const auto& text : playerMoneyTexts) {
        window.draw(text);
    }
    for (const auto& text : playerBetTexts) {
        window.draw(text);
    }

    if (bettingPhase) {
        // Draw betting buttons
        buttons[2].draw(window);  // Bet button
        buttons[3].draw(window);  // Deal button
    } else {
        // Draw all players' cards
        for (size_t i = 0; i < players.size(); ++i) {
            const auto& playerHand = players[i].getHand();
            for (const auto& card : playerHand) {
                card->draw(window, font);
            }
        }
        
        // Draw dealer's cards
        const auto& dealerHand = dealer.getHand();
        for (size_t i = 0; i < dealerHand.size(); ++i) {
            dealerHand[i]->draw(window, font, dealerRevealed || i == 0);
        }

        // Draw game buttons
        buttons[0].draw(window);  // Hit button
        buttons[1].draw(window);  // Stand button

        // Draw scores
        for (const auto& text : playerScoreTexts) {
            window.draw(text);
        }
        window.draw(dealerScoreText);
    }

    // Draw message and current player indicator
    if (gameOver || bettingPhase) {
        window.draw(messageText);
    }
    if (!gameOver) {
        window.draw(currentPlayerText);
    }

    window.display();
}