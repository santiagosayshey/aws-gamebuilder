#pragma once
#include <SFML/Graphics.hpp>

class Card
{
public:
    Card(int value);
    void setPosition(const sf::Vector2f &pos);
    void draw(sf::RenderWindow &window, const sf::Font &font, bool faceUp = true);
    int getValue() const { return value; };

private:
    sf::RectangleShape shape;
    sf::Text text;
    int value;
};