#include "Game.hpp"
#include "states/MenuState.hpp"
#include "states/GameState.hpp"
#include "states/GameSettingsState.hpp"
#include "states/HelpState.hpp"
#include "states/BettingState.hpp"
#include <iostream>

Game::Game()
    : window(sf::VideoMode(1920, 1080), "Blazing Aces") {
    window.setFramerateLimit(60);
    currentState = std::make_unique<MenuState>(window);
}

void Game::run() {
    while (window.isOpen()) {
        if (!currentState) {
            std::cerr << "Invalid state!" << std::endl;
            break;
        }
        
        currentState->handleInput();
        currentState->update();
        currentState->render();
        
        // Handle state changes
        StateChange stateChange = currentState->getStateChange();
        if (stateChange != StateChange::None) {
            std::cout << "State change requested" << std::endl;
            try {
                switch(stateChange) {
                    case StateChange::Menu:
                        std::cout << "Changing to Menu state" << std::endl;
                        changeState(std::make_unique<MenuState>(window));
                        break;
                    case StateChange::Settings:
                        std::cout << "Changing to Settings state" << std::endl;
                        changeState(std::make_unique<GameSettingsState>(window));
                        break;
                    case StateChange::Betting: {
                        std::cout << "Changing to Betting state" << std::endl;
                        const GameSettings& settings = 
                            dynamic_cast<GameSettingsState*>(currentState.get())->getSettings();
                        changeState(std::make_unique<BettingState>(window, settings));
                        break;
                    }
                    case StateChange::Game: {
                        std::cout << "Changing to Game state" << std::endl;
                        const GameSettings& settings =
                            dynamic_cast<BettingState*>(currentState.get())->getSettings();
                        const std::vector<PlayerBet>& bets =
                            dynamic_cast<BettingState*>(currentState.get())->getPlayerBets();
                        changeState(std::make_unique<GameState>(window, settings, bets));
                        break;
                    }
                    case StateChange::Help:
                        std::cout << "Changing to Help state" << std::endl;
                        changeState(std::make_unique<HelpState>(window));
                        break;
                    case StateChange::Pause:
                        std::cout << "Changing to Pause state" << std::endl;
                        // Will add PauseState later
                        break;
                    case StateChange::None:
                    default:
                        break; // No state change needed
                }
            } catch (const std::exception& e) {
                std::cerr << "Error during state change: " << e.what() << std::endl;
                window.close();
                break;
            }
        }
    }
}

void Game::changeState(std::unique_ptr<State> newState) {
    if (newState) {
        currentState = std::move(newState);
    } else {
        std::cerr << "Attempted to change to null state!" << std::endl;
        window.close();
    }
}