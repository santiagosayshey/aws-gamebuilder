#pragma once
#include "State.hpp"
#include "../ui/Button.hpp"
#include <vector>
#include <string>

class HelpState : public State {
public:
    HelpState(sf::RenderWindow& window);
    void handleInput() override;
    void update() override;
    void render() override;

private:
    void loadResources();
    void initializeButtons();
    void initializeText();
    sf::Vector2f getMousePosition() const;
    sf::Font contentFont;  // For help content
    sf::Font buttonFont;   // For button
    sf::Text titleText;
    std::vector<sf::Text> helpTexts;
    std::vector<Button> buttons;  // Back button
    
    struct Section {
        std::string title;
        std::vector<std::string> lines;
    };
    std::vector<Section> sections;

    float currentScroll = 0.f;
    const float scrollSpeed = 400.f;  // Pixels per second
};