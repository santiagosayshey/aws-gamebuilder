// Card.hpp
#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <memory>

enum class Suit
{
    Hearts,
    Diamonds,
    Clubs,
    Spades
};

class Card
{
public:
    Card(int value, Suit suit);
    void setPosition(const sf::Vector2f &pos);
    void draw(sf::RenderWindow &window, const sf::Font &font, bool faceUp = true);
    int getValue() const { return value; }
    Suit getSuit() const { return suit; }
    std::string toString() const;
    bool isAce() const { return value == 1; } // New method
    // Balance Shift wildcard helper method
    void setValue(int newValue);

private:
    int value;
    Suit suit;
    // Visual components
    sf::RectangleShape cardBase;
    sf::RectangleShape cornerBox1;
    sf::RectangleShape cornerBox2;
    std::vector<sf::CircleShape> suitSymbols;
    sf::Text rankText1; // Top-left
    sf::Text rankText2; // Bottom-right

    // Helper methods
    void setupVisuals();
    void createSuitSymbols();
    sf::Color getSuitColor() const;
    std::string getRankString() const;
    void updatePositions();
};