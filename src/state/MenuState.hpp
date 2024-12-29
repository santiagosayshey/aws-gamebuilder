#pragma once

#include "State.hpp"
#include <SFML/Graphics.hpp>

class MenuState : public State {
private:
    sf::Font font;
    sf::Text menuText;
    sf::Text start;
    sf::Text quit;

    bool isStartHovered;
    bool isQuitHovered;

    void handleButtonHover(sf::Vector2i mousePos);
    void handleButtonClick(sf::Vector2i mousePos);


public:
    MenuState();
    ~MenuState() override = default;

    void processEvent(sf::Event& event) override;
    void update() override;
    void draw(sf::RenderWindow& window) override; 
};

#endif