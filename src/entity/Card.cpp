// Card.cpp
#include "Card.hpp"

Card::Card(int value, Suit suit)
    : value(value)
    , suit(suit)
{
    setupVisuals();
    createSuitSymbols();
}

void Card::setupVisuals() {
    // Card base (white with slight gradient)
    cardBase.setSize(sf::Vector2f(75.f, 100.f));
    cardBase.setFillColor(sf::Color::White);
    cardBase.setOutlineThickness(2.f);
    cardBase.setOutlineColor(sf::Color(50, 50, 50));
    
    // Corner boxes
    cornerBox1.setSize(sf::Vector2f(20.f, 25.f));
    cornerBox2.setSize(sf::Vector2f(20.f, 25.f));
    cornerBox1.setFillColor(sf::Color(250, 250, 250));
    cornerBox2.setFillColor(sf::Color(250, 250, 250));
    cornerBox1.setOutlineThickness(1.f);
    cornerBox2.setOutlineThickness(1.f);
    cornerBox1.setOutlineColor(sf::Color(200, 200, 200));
    cornerBox2.setOutlineColor(sf::Color(200, 200, 200));
    
    // Setup rank texts
    rankText1.setCharacterSize(18);
    rankText2.setCharacterSize(18);
    rankText1.setFillColor(getSuitColor());
    rankText2.setFillColor(getSuitColor());
    rankText1.setStyle(sf::Text::Bold);
    rankText2.setStyle(sf::Text::Bold);
}

void Card::createSuitSymbols() {
    const float symbolSize = 12.f;
    sf::CircleShape symbol(symbolSize / 2.f);
    
    // Create different patterns based on card value
    int numSymbols = (value >= 10) ? 4 : value;
    float spacing = 15.f;
    
    for (int i = 0; i < numSymbols; ++i) {
        sf::CircleShape sym(symbolSize / 2.f);
        sym.setFillColor(getSuitColor());
        
        // For hearts and diamonds, create custom shapes
        if (suit == Suit::Hearts || suit == Suit::Diamonds) {
            sym.setPointCount(4);
            if (suit == Suit::Hearts) {
                // Rotate to make heart shape
                sym.rotate(45.f);
            }
        }
        suitSymbols.push_back(sym);
    }
}

sf::Color Card::getSuitColor() const {
    return (suit == Suit::Hearts || suit == Suit::Diamonds) 
           ? sf::Color(220, 0, 0) : sf::Color(20, 20, 20);
}

std::string Card::getRankString() const {
    switch (value) {
        case 1:  return "A";
        case 11: return "J";
        case 12: return "Q";
        case 13: return "K";
        default: return std::to_string(value);
    }
}

void Card::setPosition(const sf::Vector2f& pos) {
    cardBase.setPosition(pos);
    updatePositions();
}

void Card::updatePositions() {
    const sf::Vector2f basePos = cardBase.getPosition();
    
    // Position corner boxes
    cornerBox1.setPosition(basePos + sf::Vector2f(5.f, 5.f));
    cornerBox2.setPosition(basePos + sf::Vector2f(50.f, 70.f));
    
    // Position rank texts
    rankText1.setPosition(basePos + sf::Vector2f(8.f, 5.f));
    rankText2.setPosition(basePos + sf::Vector2f(53.f, 70.f));
    
    // Position suit symbols in a pattern
    float centerX = basePos.x + cardBase.getSize().x / 2.f;
    float centerY = basePos.y + cardBase.getSize().y / 2.f;
    
    if (suitSymbols.size() <= 4) {
        // Simple diamond pattern
        for (size_t i = 0; i < suitSymbols.size(); ++i) {
            float x = centerX + ((i % 2) * 20.f - 10.f);
            float y = centerY + ((i / 2) * 20.f - 10.f);
            suitSymbols[i].setPosition(x, y);
        }
    } else {
        // More complex pattern for higher values
        // ... implement different patterns for different card values
    }
}

void Card::draw(sf::RenderWindow& window, const sf::Font& font, bool faceUp) {
    window.draw(cardBase);
    
    if (faceUp) {
        window.draw(cornerBox1);
        window.draw(cornerBox2);
        
        rankText1.setFont(font);
        rankText2.setFont(font);
        rankText1.setString(getRankString());
        rankText2.setString(getRankString());
        window.draw(rankText1);
        window.draw(rankText2);
        
        for (auto& symbol : suitSymbols) {
            window.draw(symbol);
        }
    }
}

std::string Card::toString() const {
    return getRankString() + std::string(1, "HDCS"[static_cast<int>(suit)]);
}
// Balance Shift wildcard
void Card::setValue(int newValue) {
    value = newValue;
    setupVisuals();
}