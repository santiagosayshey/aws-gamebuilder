#include "MenuState.hpp"
#include <iostream>

MenuState::MenuState(sf::RenderWindow& window) 
    : State(window), selectedButton(-1) {
    loadResources();
    initializeButtons();
}

void MenuState::loadResources() {
    // Load font
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

    // Setup title text
    titleText.setFont(font);
    titleText.setString("Blackjack");
    titleText.setCharacterSize(72);
    titleText.setFillColor(sf::Color::White);
    
    // Center the title
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setOrigin(titleBounds.width / 2, titleBounds.height / 2);
    titleText.setPosition(
        window.getSize().x / 2.f,
        window.getSize().y / 4.f
    );
}

void MenuState::initializeButtons() {
    const float buttonWidth = 200.f;
    const float buttonHeight = 50.f;
    const float buttonSpacing = 20.f;
    const float startY = window.getSize().y / 2.f;

    std::vector<std::string> buttonTexts = {"Play", "Options", "Quit"};
    
    for (size_t i = 0; i < buttonTexts.size(); ++i) {
        float yPos = startY + i * (buttonHeight + buttonSpacing);
        buttons.emplace_back(
            sf::Vector2f((window.getSize().x - buttonWidth) / 2.f, yPos),
            sf::Vector2f(buttonWidth, buttonHeight),
            buttonTexts[i],
            font
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
            // Update button highlights
            for (size_t i = 0; i < buttons.size(); ++i) {
                buttons[i].setHighlight(buttons[i].isMouseOver(mousePos));
            }
        }

        if (event.type == sf::Event::MouseButtonPressed && 
            event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos = getMousePosition();
            
            // Check button clicks
            for (size_t i = 0; i < buttons.size(); ++i) {
                if (buttons[i].isMouseOver(mousePos)) {
                    switch (i) {
                        case 0: // Play
                            requestStateChange(StateChange::Game);
                            break;
                        case 1: // Options
                            // Will implement later
                            break;
                        case 2: // Quit
                            window.close();
                            break;
                    }
                }
            }
        }
    }
}

void MenuState::update() {
    // Add any menu animations or updates here
}

void MenuState::render() {
    window.clear(sf::Color(0, 100, 0)); // Dark green background

    // Draw title
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