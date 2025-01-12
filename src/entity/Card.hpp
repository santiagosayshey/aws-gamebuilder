#pragma once

#include <SFML/Graphics.hpp>
#include <string>

// We'll define a simple Suit enum for demonstration
enum class Suit {
    Hearts,    // 0
    Diamonds,  // 1
    Clubs,     // 2
    Spades     // 3
};

class Card {
public:
    // Now constructor takes both value (1–13) and suit.
    // 1=Ace, 11=Jack, 12=Queen, 13=King, or anything in between.
    Card(int value, Suit suit);

    void setPosition(const sf::Vector2f& pos);
    void draw(sf::RenderWindow& window, const sf::Font& font, bool faceUp = true);

    // Basic getters
    int getValue() const { return value; }
    Suit getSuit() const { return suit; }
    
    // Convert to short text like "A♠" or "10H"
    std::string toString() const;

private:
    int value;   // 1–13
    Suit suit;

    // SFML shapes/text if you want to visually draw a card
    sf::RectangleShape shape;
    sf::Text text;
};
