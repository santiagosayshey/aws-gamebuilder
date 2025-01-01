#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Button {
public:
    Button(const sf::Vector2f& position, const sf::Vector2f& size, const std::string& text, const sf::Font& font, const std::string& textureFile);
    void draw(sf::RenderWindow& window);
    bool isMouseOver(const sf::Vector2f& mousePos) const;
    void setHighlight(bool highlight);

private:
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Text text;
    sf::Color textDefaultColor;
    sf::Color textHoverColor;
};
