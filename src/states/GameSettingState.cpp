#include "GameSettingsState.hpp"
#include <iostream>      // <--- For the minimal logging
#include <cmath>
#include <algorithm>     // for std::clamp

GameSettingsState::GameSettingsState(sf::RenderWindow& window)
    : State(window)
    , ready(false)
    , isDraggingSlider(false)
{
    // Default settings
    settings.numPlayers = 2;
    settings.startingMoney = 500.0f;
    settings.minBet = 10.0f;
    settings.wildcardEnabled = false;

    if (!loadResources())
    {
        window.close();
        return;
    }

    initializeDecorations();
    initializeButtons();
    updateSettingsText();
}

bool GameSettingsState::loadResources()
{
    // Load title font (casino style)
    if (!titleFont.loadFromFile("src/assets/font/Casino.ttf")) {
        std::cerr << "Error: Could not load title font file.\n";
        return false;
    }

    // Load button font
    if (!buttonFont.loadFromFile("src/assets/font/Troska.ttf")) {
        std::cerr << "Error: Could not load button font file.\n";
        return false;
    }

    // Load standard font for settings text
    std::vector<std::string> fontPaths = {
        "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf", 
        "/Library/Fonts/Verdana.ttf",                                     
        "C:/Windows/Fonts/verdana.ttf",                                   
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",                
        "arial.ttf",                                                      
        "C:/Windows/Fonts/arial.ttf",                                     
        "/System/Library/Fonts/Helvetica.ttf"
    };
    
    bool standardFontLoaded = false;
    for (const auto &path : fontPaths)
    {
        if (standardFont.loadFromFile(path))
        {
            standardFontLoaded = true;
            break;
        }
    }
    
    if (!standardFontLoaded) {
        std::cerr << "Error: Could not load standard font.\n";
        return false;
    }

    // Setup title text styling
    titleText.setFont(titleFont);
    titleText.setString("GAME SETTINGS");
    titleText.setCharacterSize(80);
    titleText.setLetterSpacing(2.0f);
    titleText.setFillColor(sf::Color(255, 215, 0)); // Gold
    titleText.setOutlineThickness(3);
    titleText.setOutlineColor(sf::Color(139, 69, 19)); // Dark brown
    
    // Center the title
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setOrigin(titleBounds.width / 2, titleBounds.height / 2);
    titleText.setPosition(window.getSize().x / 2.f, window.getSize().y / 4.f);

    // Prepare setting texts
    settingTexts.resize(4);
    float startY = window.getSize().y / 2.f - 100.f;
    float spacing = 100.f;

    for (auto &text : settingTexts)
    {
        text.setFont(standardFont);
        text.setCharacterSize(32);
        text.setFillColor(sf::Color(220, 220, 220));
        text.setOutlineThickness(1);
        text.setOutlineColor(sf::Color(0, 50, 0));
        text.setPosition(window.getSize().x / 2.f - 350.f, startY);
        startY += spacing;
    }

    return true;
}


void GameSettingsState::initializeDecorations() {
    // Example decorative circles
    CircleData circle1;
    circle1.shape.setRadius(200.f);
    circle1.basePos = sf::Vector2f(-100.f, -100.f);
    circle1.xFreq = 0.8f;
    circle1.yFreq = 1.5f;
    circle1.xAmp = 15.0f;
    circle1.yAmp = 20.0f;
    circle1.phase = 0.0f;
    circle1.shape.setFillColor(sf::Color(0, 70, 0));

    CircleData circle2;
    circle2.shape.setRadius(150.f);
    circle2.basePos = sf::Vector2f(window.getSize().x - 100.f, window.getSize().y - 100.f);
    circle2.xFreq = 0.9f;
    circle2.yFreq = 1.2f;
    circle2.xAmp = 20.0f;
    circle2.yAmp = 25.0f;
    circle2.phase = 0.5f;
    circle2.shape.setFillColor(sf::Color(0, 70, 0));

    decorativeCircles.push_back(circle1);
    decorativeCircles.push_back(circle2);
}

void GameSettingsState::initializeButtons()
{
    float startY = window.getSize().y / 2.f - 100.f;
    float spacing = 100.f;
    const float adjustButtonWidth = 60.f;
    const float adjustButtonHeight = 50.f;


    // For the first 3 settings
    for (int i = 0; i < 3; ++i) {
        // Minus
        auto minusButton = Button(
            sf::Vector2f(window.getSize().x / 2.f + 100.f, startY - 10.f),
            sf::Vector2f(adjustButtonWidth, adjustButtonHeight),
            "",
            buttonFont,
            sf::Color(220, 220, 220),
            sf::Color(255, 215, 0)
        );
        sf::RectangleShape minusShape;
        minusShape.setSize(sf::Vector2f(20.f, 4.f));
        minusShape.setFillColor(sf::Color::White);
        minusShape.setOrigin(minusShape.getSize().x / 2.f, minusShape.getSize().y / 2.f);
        minusShape.setPosition(

            minusButton.getPosition().x + adjustButtonWidth / 2.f,
            minusButton.getPosition().y + adjustButtonHeight / 2.f
        );
        minusSymbols.push_back(minusShape);
        buttons.push_back(minusButton);

        // Plus
        auto plusButton = Button(
            sf::Vector2f(window.getSize().x / 2.f + 180.f, startY - 10.f),
            sf::Vector2f(adjustButtonWidth, adjustButtonHeight),
            "",
            buttonFont,
            sf::Color(220, 220, 220),
            sf::Color(255, 215, 0)
        );
        sf::RectangleShape plusVertical;
        plusVertical.setSize(sf::Vector2f(4.f, 20.f));
        plusVertical.setFillColor(sf::Color::White);
        plusVertical.setOrigin(plusVertical.getSize().x / 2.f, plusVertical.getSize().y / 2.f);
        plusVertical.setPosition(
            plusButton.getPosition().x + adjustButtonWidth / 2.f,
            plusButton.getPosition().y + adjustButtonHeight / 2.f
        );

        sf::RectangleShape plusHorizontal;
        plusHorizontal.setSize(sf::Vector2f(20.f, 4.f));
        plusHorizontal.setFillColor(sf::Color::White);
        plusHorizontal.setOrigin(plusHorizontal.getSize().x / 2.f, plusHorizontal.getSize().y / 2.f);
        plusHorizontal.setPosition(
            plusButton.getPosition().x + adjustButtonWidth / 2.f,
            plusButton.getPosition().y + adjustButtonHeight / 2.f
        );

        plusVerticalSymbols.push_back(plusVertical);
        plusHorizontalSymbols.push_back(plusHorizontal);
        buttons.push_back(plusButton);

        startY += spacing;
    }

    // Slider for wildcard toggle
    const float sliderWidth = 120.f;
    const float sliderHeight = 8.f;
    const float handleSize = 24.f;

    sliderTrack.setSize(sf::Vector2f(sliderWidth, sliderHeight));
    sliderTrack.setPosition(window.getSize().x / 2.f + 100.f, startY + 15.f);
    sliderTrack.setFillColor(sf::Color(100, 100, 100));

    sliderHandle.setSize(sf::Vector2f(handleSize, handleSize));
    sliderHandle.setOrigin(handleSize / 2.f, handleSize / 2.f);
    sliderHandle.setPosition(
        settings.wildcardEnabled ? sliderTrack.getPosition().x + sliderWidth
                                 : sliderTrack.getPosition().x,
        sliderTrack.getPosition().y + sliderHeight / 2.f
    );
    sliderHandle.setFillColor(sf::Color::White);

    // "START GAME" button
    buttons.emplace_back(
        sf::Vector2f((window.getSize().x - 300.f) / 2.f, window.getSize().y - 150.f),
        sf::Vector2f(300.f, 70.f),
        "START GAME",
        buttonFont,
        sf::Color(220, 220, 220),
        sf::Color(255, 215, 0));

    // "BACK" button
    buttons.emplace_back(
        sf::Vector2f(50.f, window.getSize().y - 150.f),
        sf::Vector2f(200.f, 70.f),
        "BACK",
        buttonFont,
        sf::Color(220, 220, 220),
        sf::Color(255, 215, 0));
}

void GameSettingsState::updateSettingsText()
{
    settingTexts[0].setString("Players: " + std::to_string(settings.numPlayers));
    settingTexts[1].setString("Starting Cash: $" + std::to_string((int)settings.startingMoney));
    settingTexts[2].setString("Minimum Bet: $" + std::to_string((int)settings.minBet));
    settingTexts[3].setString("Wildcards: " + std::string(settings.wildcardEnabled ? "On" : "Off"));

    for (auto& text : settingTexts) {
        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin(0.f, bounds.height / 2.f);
    }
}

void GameSettingsState::adjustSetting(int settingIndex, bool increase) {
    switch (settingIndex) {
    case 0: // numPlayers
        if (increase && settings.numPlayers < 6) {
            settings.numPlayers++;
        } else if (!increase && settings.numPlayers > 2) {
            settings.numPlayers--;
        }
        break;
    case 1: // startingMoney
        if (increase) {
            settings.startingMoney += 100;
        } else if (!increase && settings.startingMoney > 100) {
            settings.startingMoney -= 100;
        }
        break;
    case 2: // minBet
        if (increase && settings.minBet < settings.startingMoney / 2) {
            settings.minBet += 5;
        } else if (!increase && settings.minBet > 5) {
            settings.minBet -= 5;
        }
        break;
    }
    updateSettingsText();
}

sf::Vector2f GameSettingsState::getMousePosition() const {
    return window.mapPixelToCoords(sf::Mouse::getPosition(window));
}

void GameSettingsState::handleInput() {
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }
        
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos = getMousePosition();

            // Check minus/plus
            for (size_t i = 0; i < 3; ++i) {
                if (buttons[i * 2].isMouseOver(mousePos)) {
                    adjustSetting((int)i, false);
                } else if (buttons[i * 2 + 1].isMouseOver(mousePos)) {
                    adjustSetting((int)i, true);
                }
            }

            // Slider handle or track
            if (sliderHandle.getGlobalBounds().contains(mousePos)) {
                isDraggingSlider = true;
            }
            else if (sliderTrack.getGlobalBounds().contains(mousePos)) {
                float clickRatio = (mousePos.x - sliderTrack.getPosition().x) / sliderTrack.getSize().x;
                settings.wildcardEnabled = (clickRatio >= 0.5f);
                sliderHandle.setPosition(
                    settings.wildcardEnabled ? sliderTrack.getPosition().x + sliderTrack.getSize().x
                                             : sliderTrack.getPosition().x,
                    sliderHandle.getPosition().y
                );
                updateSettingsText();
            }

            // Check "START GAME" button
            if (buttons[buttons.size() - 2].isMouseOver(mousePos)) {
                // MINIMAL LOG: EXACTLY WHEN THE BUTTON IS PRESSED
                std::cout << "[GameSettingsState] START GAME button clicked\n";

                requestStateChange(StateChange::Betting);

                // Another small log, after requesting:
                std::cout << "[GameSettingsState] requestStateChange(Game) done\n";
            }
            // Check "BACK" button
            else if (buttons.back().isMouseOver(mousePos)) {
                requestStateChange(StateChange::Menu);
            }
        }

        if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
            if (isDraggingSlider) {
                isDraggingSlider = false;
                float sliderPos = (sliderHandle.getPosition().x - sliderTrack.getPosition().x)
                                  / sliderTrack.getSize().x;
                settings.wildcardEnabled = (sliderPos >= 0.5f);
                sliderHandle.setPosition(
                    settings.wildcardEnabled ? sliderTrack.getPosition().x + sliderTrack.getSize().x
                                             : sliderTrack.getPosition().x,
                    sliderHandle.getPosition().y
                );
                updateSettingsText();
            }
        }

        // Mouse move
        if (event.type == sf::Event::MouseMoved) {
            sf::Vector2f mousePos = getMousePosition();
            for (auto& b : buttons) {
                b.setHighlight(b.isMouseOver(mousePos));
            }
            if (isDraggingSlider) {
                float newX = std::clamp(
                    mousePos.x,
                    sliderTrack.getPosition().x,
                    std::min(mousePos.x, sliderTrack.getPosition().x + sliderTrack.getSize().x));
                sliderHandle.setPosition(newX, sliderHandle.getPosition().y);
            }
        }
    }
}

void GameSettingsState::update() {
    static float time = 0.f;
    const float deltaTime = 0.016f; // ~60 FPS
    time += deltaTime;

    float titleOffset = std::sin(time * 2.f) * 5.f;
    titleText.setPosition(window.getSize().x / 2.f, window.getSize().y / 4.f + titleOffset);

    // Circles
    for (auto& circle : decorativeCircles) {
        float xOffset = std::sin(time * circle.xFreq + circle.phase) * circle.xAmp;
        float yOffset = std::cos(time * circle.yFreq + circle.phase) * circle.yAmp;
        float circularX = std::cos(time * 0.5f + circle.phase) * (circle.xAmp * 0.5f);
        float circularY = std::sin(time * 0.5f + circle.phase) * (circle.yAmp * 0.5f);

        circle.shape.setPosition(
            circle.basePos.x + xOffset + circularX,
            circle.basePos.y + yOffset + circularY
        );
    }

    float dt = deltaTime;
    for (auto& b : buttons) {
        b.update(dt);
    }
}

void GameSettingsState::render() {
    window.clear(sf::Color(0, 60, 0));

    for (const auto& circle : decorativeCircles) {
        window.draw(circle.shape);
    }

    // Title + shadow
    sf::Text shadowText = titleText;
    shadowText.setFillColor(sf::Color(0, 40, 0));
    shadowText.setPosition(titleText.getPosition() + sf::Vector2f(4, 4));
    window.draw(shadowText);
    window.draw(titleText);

    for (auto& text : settingTexts) {
        window.draw(text);
    }

    for (auto& button : buttons) {
        button.draw(window);
    }

    // Draw minus/plus
    for (size_t i = 0; i < 3; ++i) {
        window.draw(minusSymbols[i]);
        window.draw(plusVerticalSymbols[i]);
        window.draw(plusHorizontalSymbols[i]);
    }

    window.draw(sliderTrack);
    window.draw(sliderHandle);

    window.display();
}
