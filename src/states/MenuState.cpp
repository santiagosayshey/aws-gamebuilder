#include "MenuState.hpp"

MenuState::MenuState(sf::RenderWindow& window) 
    : State(window) {
}

void MenuState::handleInput() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
    }
}

void MenuState::update() {
    // Update menu logic
}

void MenuState::render() {
    window.clear(sf::Color::Black);
    // Render menu items
    window.display();
}