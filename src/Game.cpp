#include "Game.hpp"
#include "states/MenuState.hpp"
#include "states/GameState.hpp"

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
        
        // Handle state changes
        StateChange stateChange = currentState->getStateChange();
        if (stateChange != StateChange::None) {
            switch(stateChange) {
                case StateChange::Menu:
                    changeState(std::make_unique<MenuState>(window));
                    break;
                case StateChange::Game:
                    changeState(std::make_unique<GameState>(window));
                    break;
                case StateChange::Pause:
                    // Will add PauseState later
                    break;
            }
        }
    }
}

void Game::changeState(std::unique_ptr<State> newState) {
    currentState = std::move(newState);
}