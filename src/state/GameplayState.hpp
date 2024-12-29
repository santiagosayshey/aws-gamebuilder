// GameplayState.hpp
#pragma once
#include "State.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <memory>

class GameplayState : public State {
private:
    sf::RenderWindow& window;
    bool isPaused;

public:
    GameplayState(sf::RenderWindow& window);
    void processEvent(sf::Event& event) override;
    void update() override;
    void draw(sf::RenderWindow& window) override;
};
