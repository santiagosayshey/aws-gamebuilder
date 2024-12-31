#pragma once
#include <SFML/Graphics.hpp>

class State {
public:
    State(sf::RenderWindow& window) : window(window) {}
    virtual ~State() = default;
    
    virtual void handleInput() = 0;
    virtual void update() = 0;
    virtual void render() = 0;

protected:
    sf::RenderWindow& window;
};