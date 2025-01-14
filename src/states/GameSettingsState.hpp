#pragma once
#include "State.hpp"
#include "../ui/Button.hpp"
#include <vector>
#include <string>
#include <iostream>         // for logging
#include <SFML/Graphics.hpp>

// Simple struct that holds user-chosen game settings
struct GameSettings {
    int numPlayers;
    float startingMoney;
    float minBet;
    bool wildcardEnabled;
};

// Structure for animated decorative elements
struct CircleData {
    sf::CircleShape shape;
    sf::Vector2f basePos;
    float xFreq, yFreq;
    float xAmp, yAmp;
    float phase;
};

class GameSettingsState : public State {
public:
    GameSettingsState(sf::RenderWindow& window);
    
    void handleInput() override;
    void update() override;
    void render() override;

    const GameSettings& getSettings() const { return settings; }

private:
    void initializeButtons();
    void initializeDecorations();
    bool loadResources();

    sf::Vector2f getMousePosition() const;
    void updateSettingsText();
    void adjustSetting(int settingIndex, bool increase);

    // Fonts
    sf::Font titleFont;     
    sf::Font buttonFont;    
    sf::Font standardFont;  
    
    // Slider elements
    sf::RectangleShape sliderTrack;
    sf::RectangleShape sliderHandle;
    bool isDraggingSlider;

    // Visual
    sf::Text titleText;
    std::vector<Button> buttons;
    std::vector<sf::Text> settingTexts;
    std::vector<CircleData> decorativeCircles;
    std::vector<sf::RectangleShape> minusSymbols;
    std::vector<sf::RectangleShape> plusVerticalSymbols;
    std::vector<sf::RectangleShape> plusHorizontalSymbols;
    
    // Game settings
    GameSettings settings;
    bool ready;
};
