#include "Button.hpp"
#include <iostream>

Button::Button(const sf::Vector2f& position, const sf::Vector2f& size, const std::string& buttonText, const sf::Font& font, const std::string& textureFile) {
    // Load texture
    if (!texture.loadFromFile(textureFile)) {
        std::cerr << "Error: Could not load button texture from " << textureFile << std::endl;
    }
    sprite.setTexture(texture);
    sprite.setPosition(position);

    // Scale sprite to fit the size
    sprite.setScale(size.x / texture.getSize().x, size.y / texture.getSize().y);

    // Setup text
    text.setFont(font);
    text.setString(buttonText);
    text.setCharacterSize(24);
    textDefaultColor = sf::Color::White;
    textHoverColor = sf::Color::Yellow;
    text.setFillColor(textDefaultColor);

    // Center text on the button
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin(textBounds.width / 2, textBounds.height / 2);
    text.setPosition(
        position.x + size.x / 2,
        position.y + size.y / 2
    );
}

void Button::draw(sf::RenderWindow& window) {
    window.draw(sprite);
    window.draw(text);
}

bool Button::isMouseOver(const sf::Vector2f& mousePos) const {
    return sprite.getGlobalBounds().contains(mousePos);
}

void Button::setHighlight(bool highlight) {
    text.setFillColor(highlight ? textHoverColor : textDefaultColor);
}
