#include "Game.hpp"
#include "states/MenuState.hpp"

Game::Game() 
    : window(sf::VideoMode(800, 600), "Blackjack") {
    window.setFramerateLimit(60);
    currentState = std::make_unique<MenuState>(window);
}

void Game::run() {
    while (window.isOpen()) {
        currentState->handleInput();
        currentState->update();
        currentState->render();
    }
}