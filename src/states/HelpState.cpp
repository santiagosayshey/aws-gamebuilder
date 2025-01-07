#include "HelpState.hpp"
#include <iostream>

HelpState::HelpState(sf::RenderWindow& window) 
    : State(window) {
    loadResources();
    initializeText();
    initializeButtons();
}

void HelpState::loadResources() {
    if (!font.loadFromFile("src/assets/font/Troska.ttf")) {
        std::cerr << "Error: Could not load font file." << std::endl;
        return;
    }

    // Setup title
    titleText.setFont(font);
    titleText.setString("How to Play");
    titleText.setCharacterSize(48);
    titleText.setFillColor(sf::Color(220, 220, 220));
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setPosition(
        (window.getSize().x - titleBounds.width) / 2.f,
        30.f
    );
}

void HelpState::initializeText() {
    // Define sections with their content
    sections = {
        {"Basic Rules", {
            "• Try to get closer to 21 than the dealer without going over",
            "• Aces are worth 1 or 11",
            "• Face cards are worth 10",
            "• Place your bet before each hand",
            "• Hit to take another card, Stand to keep your hand"
        }},
        {"Betting", {
            "• Minimum bet varies based on settings",
            "• Win pays 1:1",
            "• Blackjack (21 with first two cards) pays 3:2",
            "• Push (tie) returns your bet"
        }},
        {"Wildcards", {
            "• Special abilities can be enabled in settings",
            "• Card Swap: Exchange one of your cards with a new one",
            "• Foresight: Peek at the dealer's face-down card",
            "• Each wildcard can only be used once per game"
        }},
        {"Dealer Rules", {
            "• Dealer must hit on 16 and below",
            "• Dealer must stand on 17 and above",
            "• Dealer's first card is face-up, second is face-down"
        }},
        {"Winning", {
            "• Get a higher total than the dealer without busting",
            "• Dealer busts (goes over 21)",
            "• Get a blackjack (Ace + 10-value card)"
        }}
    };

    float yPos = 100.f;  // Start below title
    const float sectionSpacing = 40.f;
    const float lineSpacing = 30.f;

    // Create text objects for each section
    for (const auto& section : sections) {
        // Section title
        sf::Text sectionTitle;
        sectionTitle.setFont(font);
        sectionTitle.setString(section.title);
        sectionTitle.setCharacterSize(36);
        sectionTitle.setFillColor(sf::Color(255, 215, 0));  // Gold color
        sectionTitle.setPosition(50.f, yPos);
        helpTexts.push_back(sectionTitle);
        
        yPos += 50.f;  // Space after title

        // Section content
        for (const auto& line : section.lines) {
            sf::Text lineText;
            lineText.setFont(font);
            lineText.setString(line);
            lineText.setCharacterSize(24);
            lineText.setFillColor(sf::Color(220, 220, 220));
            lineText.setPosition(70.f, yPos);
            helpTexts.push_back(lineText);
            
            yPos += lineSpacing;
        }
        
        yPos += sectionSpacing;  // Space between sections
    }
}

void HelpState::initializeButtons() {
    // Back button
    buttons.emplace_back(
        sf::Vector2f(50.f, window.getSize().y - 70.f),
        sf::Vector2f(120.f, 50.f),
        "Back",
        font,
        sf::Color(220, 220, 220),
        sf::Color(255, 215, 0)
    );
}

void HelpState::handleInput() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        // Handle scrolling
        if (event.type == sf::Event::MouseWheelScrolled) {
            currentScroll += event.mouseWheelScroll.delta * scrollSpeed;
            // Limit scrolling
            float maxScroll = 0.f;  // Can't scroll up past the top
            float minScroll = -std::max(0.f, helpTexts.back().getPosition().y - (window.getSize().y - 100.f));
            currentScroll = std::min(maxScroll, std::max(minScroll, currentScroll));
        }
        
        if (event.type == sf::Event::MouseMoved) {
            sf::Vector2f mousePos = getMousePosition();
            for (auto& button : buttons) {
                button.setHighlight(button.isMouseOver(mousePos));
            }
        }

        if (event.type == sf::Event::MouseButtonPressed && 
            event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos = getMousePosition();
            
            if (buttons[0].isMouseOver(mousePos)) {  // Back button
                requestStateChange(StateChange::Menu);
                return;
            }
        }
    }
}

void HelpState::update() {
    for (auto& button : buttons) {
        button.update(0.016f);  // Fixed time step
    }
}

void HelpState::render() {
    window.clear(sf::Color(0, 60, 0));  // Dark green background
    
    // Create a view for scrolling
    sf::View view = window.getDefaultView();
    view.move(0, -currentScroll);
    window.setView(view);

    // Draw help texts
    for (const auto& text : helpTexts) {
        window.draw(text);
    }

    // Reset view for fixed elements
    window.setView(window.getDefaultView());

    // Draw title and buttons (fixed position)
    window.draw(titleText);
    for (auto& button : buttons) {
        button.draw(window);
    }

    window.display();
}

sf::Vector2f HelpState::getMousePosition() const {
    return window.mapPixelToCoords(sf::Mouse::getPosition(window));
}