#pragma once
#include "State.hpp"
#include "../ui/Button.hpp"
#include <vector>
#include <string>

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
    // Initialization methods
    void initializeButtons();
    void initializeDecorations();
    bool loadResources();
    
    // Helper methods
    sf::Vector2f getMousePosition() const;
    void updateSettingsText();
    void adjustSetting(int settingIndex, bool increase);

    // Fonts
    sf::Font titleFont;     // Casino-style font for title
    sf::Font buttonFont;    // Font for buttons
    sf::Font standardFont;  // Standard font for settings text
    
    // Slider elements for wildcards
    sf::RectangleShape sliderTrack;
    sf::RectangleShape sliderHandle;
    bool isDraggingSlider;
    
    // Visual elements
    sf::Text titleText;
    std::vector<Button> buttons;
    std::vector<sf::Text> settingTexts;
    std::vector<CircleData> decorativeCircles;  // Animated background elements
    std::vector<sf::RectangleShape> minusSymbols;      // Shapes for minus buttons
    std::vector<sf::RectangleShape> plusVerticalSymbols;   // Vertical lines for plus buttons
    std::vector<sf::RectangleShape> plusHorizontalSymbols; // Horizontal lines for plus buttons
    
    // Game settings
    GameSettings settings;
    bool ready;
};