#pragma once
#include <SFML/Graphics.hpp>
#include "states/State.hpp"
#include <memory>

class Game {
public:
    Game();
    ~Game() = default;
    
    void run();
    void changeState(std::unique_ptr<State> newState);

private:
    sf::RenderWindow window;
    std::unique_ptr<State> currentState;
};