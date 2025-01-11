#pragma once
#include <SFML/Graphics.hpp>

enum class Suit { HEARTS, DIAMONDS, CLUBS, SPADES };

class Card {
 public:
  Card(int value, Suit suit);
  void setPosition(const sf::Vector2f &pos);
  void draw(sf::RenderWindow &window, const sf::Font &font, bool faceUp = true);
  int getValue() const { return value; };
  Suit getSuit() const { return suit; };
  bool isAce() const;

 private:
  sf::RectangleShape shape;
  sf::Text text;
  int value;
  Suit suit;

  std::string suitToString() const;
  std::string valueToString() const;
};