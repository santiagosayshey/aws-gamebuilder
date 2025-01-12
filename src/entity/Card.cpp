#include "Card.hpp"

Card::Card(int value)
    : value(value)
{
    shape.setSize(sf::Vector2f(75.f, 100.f));
    shape.setFillColor(sf::Color::White);
    shape.setOutlineThickness(2.f);
    shape.setOutlineColor(sf::Color::Black);

    text.setCharacterSize(24);
    text.setFillColor(sf::Color::Black);
}

void Card::setPosition(const sf::Vector2f& pos) {
    shape.setPosition(pos);
    text.setPosition(pos.x + 10.f, pos.y + 10.f);
}

void Card::draw(sf::RenderWindow& window, const sf::Font& font, bool faceUp) {
    window.draw(shape);
    if (faceUp) {
        text.setFont(font);
        text.setString(std::to_string(value));
        window.draw(text);
    }
}
