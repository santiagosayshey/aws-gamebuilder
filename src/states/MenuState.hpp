#pragma once
#include "State.hpp"
#include "../ui/Button.hpp"
#include <vector>
#include <string>

class MenuState : public State {
public:
    MenuState(sf::RenderWindow& window);
    
    void handleInput() override;
    void update() override;
    void render() override;

private:
    void initializeButtons();
    void loadResources();
    sf::Vector2f getMousePosition() const;

    sf::Font titleFont;
    sf::Font buttonFont;
    sf::Text titleText;
    std::vector<Button> buttons;
    int selectedButton;
    struct CircleData {
        sf::CircleShape shape;
        sf::Vector2f basePos;
        float xFreq;
        float yFreq;
        float xAmp;
        float yAmp;
        float phase;
    };
    std::vector<CircleData> decorativeCircles;
};