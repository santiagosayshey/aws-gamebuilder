#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Button {
public:
    Button(const sf::Vector2f& position, const sf::Vector2f& size, const std::string& text, const sf::Font& font);
    void draw(sf::RenderWindow& window);
    bool isMouseOver(const sf::Vector2f& mousePos) const;
    void setHighlight(bool highlight);

private:
    sf::RectangleShape shape;
    sf::Text text;
    sf::Color defaultColor;
    sf::Color hoverColor;
};