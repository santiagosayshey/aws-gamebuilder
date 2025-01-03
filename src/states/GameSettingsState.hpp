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

class GameSettingsState : public State {
public:
    GameSettingsState(sf::RenderWindow& window);
    
    void handleInput() override;
    void update() override;
    void render() override;
    const GameSettings& getSettings() const { return settings; }

private:
    void initializeButtons();
    bool loadResources();
    sf::Vector2f getMousePosition() const;
    void updateSettingsText();
    void adjustSetting(int settingIndex, bool increase);

    sf::Font font;
    sf::Text titleText;
    std::vector<Button> buttons;
    std::vector<sf::Text> settingTexts;
    GameSettings settings;
    bool ready;
};