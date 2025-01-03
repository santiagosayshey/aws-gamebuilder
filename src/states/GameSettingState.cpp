#include "GameSettingsState.hpp"
#include <iostream>

GameSettingsState::GameSettingsState(sf::RenderWindow& window) 
    : State(window)
    , ready(false) {
    // Default settings
    settings.numPlayers = 1;
    settings.startingMoney = 500.0f;
    settings.minBet = 10.0f;
    settings.wildcardEnabled = false;
    
    if (!loadResources()) {
        window.close();
        return;
    }
    
    initializeButtons();
    updateSettingsText();
}

bool GameSettingsState::loadResources() {
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

    // Setup title
    titleText.setFont(font);
    titleText.setString("Game Settings");
    titleText.setCharacterSize(48);
    titleText.setFillColor(sf::Color::White);
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setPosition(
        (window.getSize().x - titleBounds.width) / 2.f,
        50.f
    );

    // Setup setting texts
    settingTexts.resize(4);
    float startY = 150.f;
    float spacing = 80.f;
    
    for (auto& text : settingTexts) {
        text.setFont(font);
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::White);
        text.setPosition(window.getSize().x / 2.f - 200.f, startY);
        startY += spacing;
    }

    return true;
}

void GameSettingsState::initializeButtons() {
    const std::string buttonTexture = "src/assets/ui/button.png";
    float startY = 150.f;
    float spacing = 80.f;

    // Add decrease/increase buttons for each setting
    for (int i = 0; i < 4; ++i) {
        // Decrease button
        buttons.emplace_back(
            sf::Vector2f(window.getSize().x / 2.f + 100.f, startY),
            sf::Vector2f(40.f, 40.f),
            "-",
            font,
            buttonTexture
        );

        // Increase button
        buttons.emplace_back(
            sf::Vector2f(window.getSize().x / 2.f + 150.f, startY),
            sf::Vector2f(40.f, 40.f),
            "+",
            font,
            buttonTexture
        );

        startY += spacing;
    }

    // Start button at bottom
    buttons.emplace_back(
        sf::Vector2f((window.getSize().x - 200.f) / 2.f, window.getSize().y - 100.f),
        sf::Vector2f(200.f, 50.f),
        "Start Game",
        font,
        buttonTexture
    );
}

void GameSettingsState::updateSettingsText() {
    settingTexts[0].setString("Number of Players: " + std::to_string(settings.numPlayers));
    settingTexts[1].setString("Starting Money: $" + std::to_string(int(settings.startingMoney)));
    settingTexts[2].setString("Minimum Bet: $" + std::to_string(int(settings.minBet)));
    settingTexts[3].setString("Wildcards: " + std::string(settings.wildcardEnabled ? "Enabled" : "Disabled"));
}

void GameSettingsState::adjustSetting(int settingIndex, bool increase) {
    switch(settingIndex) {
        case 0: // Number of players
            if (increase && settings.numPlayers < 4) settings.numPlayers++;
            else if (!increase && settings.numPlayers > 1) settings.numPlayers--;
            break;
        case 1: // Starting money
            if (increase) settings.startingMoney += 100;
            else if (!increase && settings.startingMoney > 100) settings.startingMoney -= 100;
            break;
        case 2: // Min bet
            if (increase && settings.minBet < settings.startingMoney / 10) settings.minBet += 5;
            else if (!increase && settings.minBet > 5) settings.minBet -= 5;
            break;
        case 3: // Wildcards
            settings.wildcardEnabled = !settings.wildcardEnabled;
            break;
    }
    updateSettingsText();
}

void GameSettingsState::handleInput() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        
        if (event.type == sf::Event::MouseButtonPressed && 
            event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos = getMousePosition();
            
            // Check setting adjustment buttons
            for (size_t i = 0; i < 4; ++i) {
                if (buttons[i*2].isMouseOver(mousePos)) {
                    adjustSetting(i, false);  // decrease
                }
                else if (buttons[i*2+1].isMouseOver(mousePos)) {
                    adjustSetting(i, true);   // increase
                }
            }

            // Check start button
            if (buttons.back().isMouseOver(mousePos)) {
                requestStateChange(StateChange::Game);
            }
        }

        // Mouse move for button highlights
        if (event.type == sf::Event::MouseMoved) {
            sf::Vector2f mousePos = getMousePosition();
            for (auto& button : buttons) {
                button.setHighlight(button.isMouseOver(mousePos));
            }
        }
    }
}

void GameSettingsState::update() {
    // Nothing to update continuously
}

void GameSettingsState::render() {
    window.clear(sf::Color(0, 100, 0));  // Dark green background

    // Draw title
    window.draw(titleText);

    // Draw setting texts
    for (const auto& text : settingTexts) {
        window.draw(text);
    }

    // Draw buttons
    for (auto& button : buttons) {
        button.draw(window);
    }

    window.display();
}

sf::Vector2f GameSettingsState::getMousePosition() const {
    return window.mapPixelToCoords(sf::Mouse::getPosition(window));
}