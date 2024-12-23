// Game.hpp
#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "state/State.hpp"



class Game {
private:
    sf::RenderWindow window;
    std::unique_ptr<State> currentState;
    
    void processEvents();
    void update();
    void render();

public:
    Game();
    void processEvents();
    void update();
    void render();
    void run();
    
    // State management
    void setState(std::unique_ptr<State> state);
};