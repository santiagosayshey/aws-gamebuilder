#pragma once
#include "State.hpp"

class MenuState : public State {
public:
    MenuState(sf::RenderWindow& window);
    
    void handleInput() override;
    void update() override;
    void render() override;
};