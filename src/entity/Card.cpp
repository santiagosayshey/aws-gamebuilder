#include "Card.hpp"
#include <sstream>

Card::Card(int value, Suit suit)
    : value(value)
    , suit(suit)
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
    // Draw the rectangle
    window.draw(shape);

    if (faceUp) {
        text.setFont(font);
        text.setString(toString());
        window.draw(text);
    }
}

std::string Card::toString() const {
    // Convert the numeric value to a rank string
    std::string rank;
    switch (value) {
        case 1:  rank = "A";  break;
        case 11: rank = "J";  break;
        case 12: rank = "Q";  break;
        case 13: rank = "K";  break;
        default: rank = std::to_string(value); break;
    }

    // Convert suit to a short char, e.g. H, D, C, S
    char suitChar;
    switch (suit) {
        case Suit::Hearts:   suitChar = 'H'; break;
        case Suit::Diamonds: suitChar = 'D'; break;
        case Suit::Clubs:    suitChar = 'C'; break;
        case Suit::Spades:   suitChar = 'S'; break;
    }

    // e.g. "10H" or "A♣"
    return rank + suitChar;
}
