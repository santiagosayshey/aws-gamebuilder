#include "MenuState.hpp"
#include <iostream>
#include <cmath>

MenuState::MenuState(sf::RenderWindow& window) 
    : State(window), selectedButton(-1) {
    loadResources();
    initializeButtons();
}

void MenuState::loadResources() {
    // Load title font
    if (!titleFont.loadFromFile("src/assets/font/Casino.ttf")) {
        std::cerr << "Error: Could not load title font file." << std::endl;
        return;
    }

    // Load button font
    if (!buttonFont.loadFromFile("src/assets/font/Troska.ttf")) {
        std::cerr << "Error: Could not load button font file." << std::endl;
        return;
    }

    // Setup title text with casino styling
    titleText.setFont(titleFont);
    titleText.setString("JACK OF FORTUNE");
    titleText.setCharacterSize(120);  // Larger size for casino font
    titleText.setLetterSpacing(2.0);  // More spacing for that Vegas feel
    titleText.setFillColor(sf::Color(0, 0, 0));  // Rich gold color
    titleText.setOutlineThickness(4);  // Thicker outline
    titleText.setOutlineColor(sf::Color(0, 50, 0));
    
    // Center the title with a slight vertical offset
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setOrigin(titleBounds.width / 2, titleBounds.height / 2);
    titleText.setPosition(
        window.getSize().x / 2.f,
        window.getSize().y / 3.f
    );
}

void MenuState::initializeButtons() {
    const float buttonWidth = 280.f;  // Slightly wider for better appearance
    const float buttonHeight = 70.f;  // Slightly taller for better appearance
    const float buttonSpacing = 40.f;
    const float startY = window.getSize().y / 2.f + 50.f;

    std::vector<std::pair<std::string, sf::Vector2f>> buttonConfigs = {
        {"PLAY", {1.0f, 1.0f}},
        {"HELP", {0.9f, 0.9f}},
        {"EXIT", {0.9f, 0.9f}}
    };
    
    for (size_t i = 0; i < buttonConfigs.size(); ++i) {
        float yPos = startY + i * (buttonHeight + buttonSpacing);
        float scaledWidth = buttonWidth * buttonConfigs[i].second.x;
        float scaledHeight = buttonHeight * buttonConfigs[i].second.y;
        float xPos = (window.getSize().x - scaledWidth) / 2.f;
        
        buttons.emplace_back(
            sf::Vector2f(xPos, yPos),             // Position
            sf::Vector2f(scaledWidth, scaledHeight), // Size
            buttonConfigs[i].first,               // Button text
            buttonFont,                           // Using buttonFont for buttons
            sf::Color(220, 220, 220),          // Default: Light silver
            sf::Color(255, 215, 0)             // Hover: Bright gold
        );
    }
}

void MenuState::handleInput() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        
        if (event.type == sf::Event::MouseMoved) {
            sf::Vector2f mousePos = getMousePosition();
            for (size_t i = 0; i < buttons.size(); ++i) {
                buttons[i].setHighlight(buttons[i].isMouseOver(mousePos));
            }
        }

        if (event.type == sf::Event::MouseButtonPressed && 
            event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos = getMousePosition();
            
            for (size_t i = 0; i < buttons.size(); ++i) {
                if (buttons[i].isMouseOver(mousePos)) {
                    switch (i) {
                        case 0: // Play
                            std::cout << "Play button clicked" << std::endl;
                            requestStateChange(StateChange::Settings);  // Changed from StateChange::Game
                            return;
                        case 1: // Help
                            // Will implement help state later
                            break;
                        case 2: // Exit
                            window.close();
                            break;
                    }
                }
            }
        }
    }
}

void MenuState::update() {
    static float time = 0.0f;
    const float deltaTime = 0.016f;  // Approximately 60 FPS
    time += deltaTime;
    
    // Update title animation
    float offset = std::sin(time * 2.0f) * 5.0f;
    titleText.setPosition(
        window.getSize().x / 2.f,
        (window.getSize().y / 3.f) + offset
    );
    
    // Update button animations
    for (auto& button : buttons) {
        button.update(deltaTime);
    }
}

void MenuState::render() {
    // Create gradient background
    sf::RectangleShape background(sf::Vector2f(window.getSize().x, window.getSize().y));
    sf::Color darkGreen(0, 60, 0);
    sf::Color lightGreen(0, 100, 0);
    
    // Draw dark gradient corners
    window.clear(darkGreen);
    
    // Draw decorative elements
    sf::CircleShape decorCircle(200.f);
    decorCircle.setFillColor(sf::Color(0, 70, 0));
    decorCircle.setPosition(-100.f, -100.f);
    window.draw(decorCircle);
    
    decorCircle.setRadius(150.f);
    decorCircle.setPosition(window.getSize().x - 100.f, window.getSize().y - 100.f);
    window.draw(decorCircle);

    // Draw title with shadow effect
    sf::Text shadowText = titleText;
    shadowText.setFillColor(sf::Color(0, 40, 0));
    shadowText.setPosition(titleText.getPosition() + sf::Vector2f(4, 4));
    window.draw(shadowText);
    window.draw(titleText);

    // Draw buttons
    for (auto& button : buttons) {
        button.draw(window);
    }

    window.display();
}

sf::Vector2f MenuState::getMousePosition() const {
    return window.mapPixelToCoords(sf::Mouse::getPosition(window));
}