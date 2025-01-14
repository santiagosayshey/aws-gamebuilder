#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Button {
public:
    Button(const sf::Vector2f& position, 
           const sf::Vector2f& size, 
           const std::string& text, 
           const sf::Font& font,
           const sf::Color& defaultColor = sf::Color::White,
           const sf::Color& hoverColor = sf::Color::Yellow);
    
    void draw(sf::RenderWindow& window);
    bool isMouseOver(const sf::Vector2f& mousePos) const;
    void setHighlight(bool highlight);
    void update(float deltaTime);
    
    // NEW: Provide a getter for the position
    sf::Vector2f getPosition() const;

private:
    sf::RectangleShape mainShape;
    sf::RectangleShape shadowShape;
    sf::RectangleShape innerGlow;
    sf::Text text;
    
    // Colors
    sf::Color baseColor;
    sf::Color highlightColor;
    sf::Color currentColor;
    sf::Color shadowColor;
    sf::Color textDefaultColor;
    sf::Color textHoverColor;
    
    float hoverTransition;
    bool isHovered;
    
    void updateColors();
    void createGradient(sf::RectangleShape& shape, 
                        const sf::Color& topColor, 
                        const sf::Color& bottomColor);
};
