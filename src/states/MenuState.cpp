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
    titleText.setString("Blazing Aces");
    titleText.setCharacterSize(80);
    titleText.setLetterSpacing(2.0f);
    titleText.setFillColor(sf::Color(255, 215, 0));  // Gold color
    titleText.setOutlineThickness(3);
    titleText.setOutlineColor(sf::Color(139, 69, 19));  // Dark brown outline for gold
    
    // Center the title with a slight vertical offset
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setOrigin(titleBounds.width / 2, titleBounds.height / 2);
    titleText.setPosition(
        window.getSize().x / 2.f,
        window.getSize().y / 3.f
    );

    // Create circles with different properties
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

    CircleData circle3;
    circle3.shape.setRadius(100.f);
    circle3.basePos = sf::Vector2f(-50.f, window.getSize().y - 150.f);
    circle3.xFreq = 1.2f;
    circle3.yFreq = 0.7f;
    circle3.xAmp = 25.0f;
    circle3.yAmp = 15.0f;
    circle3.phase = 1.0f;
    circle3.shape.setFillColor(sf::Color(0, 70, 0));

    CircleData circle4;
    circle4.shape.setRadius(120.f);
    circle4.basePos = sf::Vector2f(window.getSize().x - 50.f, -50.f);
    circle4.xFreq = 0.6f;
    circle4.yFreq = 1.1f;
    circle4.xAmp = 30.0f;
    circle4.yAmp = 20.0f;
    circle4.phase = 1.5f;
    circle4.shape.setFillColor(sf::Color(0, 70, 0));

    decorativeCircles.push_back(circle1);
    decorativeCircles.push_back(circle2);
    decorativeCircles.push_back(circle3);
    decorativeCircles.push_back(circle4);
}

void MenuState::initializeButtons() {
    const float buttonWidth = 280.f;
    const float buttonHeight = 70.f;
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
            sf::Vector2f(xPos, yPos),
            sf::Vector2f(scaledWidth, scaledHeight),
            buttonConfigs[i].first,
            buttonFont,
            sf::Color(220, 220, 220),
            sf::Color(255, 215, 0)
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
                        case 0:
                            std::cout << "Play button clicked" << std::endl;
                            requestStateChange(StateChange::Settings);
                            return;
                        case 1: // Help button
                            std::cout << "Help button clicked" << std::endl;
                            requestStateChange(StateChange::Help);
                            return;
                        case 2:
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
    float titleOffset = std::sin(time * 2.0f) * 5.0f;
    titleText.setPosition(
        window.getSize().x / 2.f,
        (window.getSize().y / 3.f) + titleOffset
    );
    
    // Update circle animations
    for (auto& circle : decorativeCircles) {
        float xOffset = std::sin(time * circle.xFreq + circle.phase) * circle.xAmp;
        float yOffset = std::cos(time * circle.yFreq + circle.phase) * circle.yAmp;
        
        // Add circular motion component
        float circularX = std::cos(time * 0.5f + circle.phase) * (circle.xAmp * 0.5f);
        float circularY = std::sin(time * 0.5f + circle.phase) * (circle.yAmp * 0.5f);
        
        circle.shape.setPosition(
            circle.basePos.x + xOffset + circularX,
            circle.basePos.y + yOffset + circularY
        );
    }
    
    // Update button animations
    for (auto& button : buttons) {
        button.update(deltaTime);
    }
}

void MenuState::render() {
    // Draw dark gradient corners
    window.clear(sf::Color(0, 60, 0));
    
    // Draw decorative elements
    for (const auto& circle : decorativeCircles) {
        window.draw(circle.shape);
    }

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