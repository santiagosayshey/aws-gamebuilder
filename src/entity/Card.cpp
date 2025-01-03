#include "Card.hpp"

Card::Card(int value) : value(value) {
    shape.setSize(sf::Vector2f(75.f, 100.f));
    shape.setFillColor(sf::Color::White);
    shape.setOutlineThickness(2);
    shape.setOutlineColor(sf::Color::Black);
}

void Card::setPosition(const sf::Vector2f& pos) {
    shape.setPosition(pos);
    text.setPosition(pos.x + 10, pos.y + 10);
}

void Card::draw(sf::RenderWindow& window, const sf::Font& font, bool faceUp) {
    window.draw(shape);
    if (faceUp) {
        text.setFont(font);
        text.setString(std::to_string(value));
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::Black);
        window.draw(text);
    }
}