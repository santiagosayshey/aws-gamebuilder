#include "MenuState.hpp"
#include <iostream>

MenuState::MenuState()
    : isStartHovered(false), isQuitHovered(false)
{
    // Load font
    if (!font.loadFromFile("font path here")) {
        std::cerr << "Error loading font!" << std::endl;
    }

    menuText.setFont(font);
    menuText.setString("Game Menu");

    start.setFont(font);
    start.setString("Start Game");

    quit.setFont(font);
    quit.setString("Quit");

}

MenuState::~MenuState() {

}

void MenuState::processEvent(sf::Event& event) {
    // code here
}

void MenuState::update() {
    // code here
}

void MenuState::draw(sf::RenderWindow& window){
    // code here
}

void MenuState::handleButtonHover(sf::Vector2i mousePos) {
    // code here
}

void MenuState::handleButtonClick(sf::Vector2i mousePos){
    // code here
}