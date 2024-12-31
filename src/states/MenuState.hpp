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

    sf::Font font;
    sf::Text titleText;
    std::vector<Button> buttons;
    int selectedButton;
};