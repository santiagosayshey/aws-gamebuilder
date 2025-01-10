// Button.hpp
#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Button {
public:
    Button(const sf::Vector2f& position, 
           const sf::Vector2f& size, 
           const std::string& text, 
           const sf::Font& font,
           const sf::Color& defaultColor = sf::Color::White,    // Default text color
           const sf::Color& hoverColor = sf::Color::Yellow);    // Default hover color
    
    void draw(sf::RenderWindow& window);
    bool isMouseOver(const sf::Vector2f& mousePos) const;
    void setHighlight(bool highlight);
    void update(float deltaTime);

private:
    // Visual elements
    sf::RectangleShape mainShape;      // Main button body
    sf::RectangleShape shadowShape;    // Drop shadow
    sf::RectangleShape innerGlow;      // Inner highlight
    sf::Text text;
    
    // Colors
    sf::Color baseColor;               // Main button color
    sf::Color highlightColor;          // Color when hovered
    sf::Color currentColor;            // Current interpolated color
    sf::Color shadowColor;             // Shadow color
    sf::Color textDefaultColor;        // Default text color
    sf::Color textHoverColor;          // Text color when hovered
    
    // Animation
    float hoverTransition;             // 0.0f to 1.0f for smooth transitions
    bool isHovered;                    // Current hover state
    
    void updateColors();               // Update gradient and colors
    void createGradient(sf::RectangleShape& shape, 
                       const sf::Color& topColor, 
                       const sf::Color& bottomColor);
};