#include "Button.hpp"

Button::Button(const sf::Vector2f& position, const sf::Vector2f& size, const std::string& buttonText, const sf::Font& font) {
    // Setup shape
    shape.setSize(size);
    shape.setPosition(position);
    defaultColor = sf::Color(0, 100, 0);     // Darker green
    hoverColor = sf::Color(0, 120, 0);       // Slightly lighter green
    shape.setFillColor(defaultColor);

    // Setup text
    text.setFont(font);
    text.setString(buttonText);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);

    // Center text
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin(textBounds.width / 2, textBounds.height / 2);
    text.setPosition(
        position.x + size.x / 2,
        position.y + size.y / 2
    );
}

void Button::draw(sf::RenderWindow& window) {
    window.draw(shape);
    window.draw(text);
}

bool Button::isMouseOver(const sf::Vector2f& mousePos) const {
    return shape.getGlobalBounds().contains(mousePos);
}

void Button::setHighlight(bool highlight) {
    shape.setFillColor(highlight ? hoverColor : defaultColor);
}