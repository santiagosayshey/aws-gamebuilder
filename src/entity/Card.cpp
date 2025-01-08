#include "Card.hpp"

Card::Card(int value, Suit suit) : value(value, suit(suit))
{
    shape.setSize(sf::Vector2f(75.f, 100.f));
    shape.setFillColor(sf::Color::White);
    shape.setOutlineThickness(2);
    shape.setOutlineColor(sf::Color::Black);
}

void Card::setPosition(const sf::Vector2f &pos)
{
    shape.setPosition(pos);
    text.setPosition(pos.x + 10, pos.y + 10);
}

void Card::draw(sf::RenderWindow &window, const sf::Font &font, bool faceUp)
{
    window.draw(shape);
    if (faceUp)
    {
        text.setFont(font);
        text.setString(std::to_string(value));
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::Black);
        window.draw(text);
    }
}

std::string Card::suitToString() const
{
    switch (suit)
    {
    case Suit::HEARTS:
        return "Hearts";
    case Suit::DIAMONDS:
        return "Diamonds";
    case Suit::CLUBS:
        return "Clubs";
    case Suit::SPADES:
        return "Spades";
    default:
        return "Unknown";
    }
}

std::string Card::valueToString() const
{
    switch (value)
    {
    case 1:
        return "A";
    case 11:
        return "J";
    case 12:
        return "Q";
    case 13:
        return "K";
    default:
        return std::to_string(value);
    }
}