// Button.cpp
#include "Button.hpp"

Button::Button(const sf::Vector2f& position, const sf::Vector2f& size, 
               const std::string& buttonText, const sf::Font& font) 
    : hoverTransition(0.0f), isHovered(false) {
    
    // Setup main button shape
    mainShape.setSize(size);
    mainShape.setPosition(position);
    
    // Base colors
    baseColor = sf::Color(20, 80, 20);          // Dark green
    highlightColor = sf::Color(30, 120, 30);    // Lighter green
    currentColor = baseColor;
    shadowColor = sf::Color(0, 20, 0, 180);     // Very dark green with alpha
    
    // Setup shadow
    shadowShape.setSize(size);
    shadowShape.setPosition(position + sf::Vector2f(4, 4));
    shadowShape.setFillColor(shadowColor);
    
    // Setup inner glow
    innerGlow.setSize(sf::Vector2f(size.x - 4, size.y - 4));
    innerGlow.setPosition(position + sf::Vector2f(2, 2));
    
    // Setup text
    text.setFont(font);
    text.setString(buttonText);
    text.setCharacterSize(30);
    textDefaultColor = sf::Color(200, 255, 200);  // Light green
    textHoverColor = sf::Color::White;
    text.setFillColor(textDefaultColor);
    text.setStyle(sf::Text::Bold);
    
    // Center text
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin(textBounds.width / 2, textBounds.height / 2);
    text.setPosition(
        position.x + size.x / 2,
        position.y + size.y / 2 - 2  // Slight upward adjustment
    );
    
    updateColors();
}

void Button::update(float deltaTime) {
    const float transitionSpeed = 6.0f;
    
    if (isHovered) {
        hoverTransition = std::min(1.0f, hoverTransition + deltaTime * transitionSpeed);
    } else {
        hoverTransition = std::max(0.0f, hoverTransition - deltaTime * transitionSpeed);
    }
    
    updateColors();
}

void Button::updateColors() {
    // Interpolate between base and highlight colors
    currentColor = sf::Color(
        baseColor.r + (highlightColor.r - baseColor.r) * hoverTransition,
        baseColor.g + (highlightColor.g - baseColor.g) * hoverTransition,
        baseColor.b + (highlightColor.b - baseColor.b) * hoverTransition
    );
    
    // Create gradients
    createGradient(mainShape, currentColor, 
                  sf::Color(currentColor.r/2, currentColor.g/2, currentColor.b/2));
    
    // Update inner glow
    sf::Color glowColor(255, 255, 255, static_cast<sf::Uint8>(40 + 40 * hoverTransition));
    createGradient(innerGlow, glowColor, sf::Color(255, 255, 255, 0));
    
    // Update text color
    text.setFillColor(sf::Color(
        textDefaultColor.r + (textHoverColor.r - textDefaultColor.r) * hoverTransition,
        textDefaultColor.g + (textHoverColor.g - textDefaultColor.g) * hoverTransition,
        textDefaultColor.b + (textHoverColor.b - textDefaultColor.b) * hoverTransition
    ));
}

void Button::createGradient(sf::RectangleShape& shape, 
                          const sf::Color& topColor, 
                          const sf::Color& bottomColor) {
    // Create vertical gradient
    sf::VertexArray gradient(sf::Quads, 4);
    sf::Vector2f pos = shape.getPosition();
    sf::Vector2f size = shape.getSize();
    
    gradient[0].position = pos;
    gradient[1].position = sf::Vector2f(pos.x + size.x, pos.y);
    gradient[2].position = pos + size;
    gradient[3].position = sf::Vector2f(pos.x, pos.y + size.y);
    
    gradient[0].color = topColor;
    gradient[1].color = topColor;
    gradient[2].color = bottomColor;
    gradient[3].color = bottomColor;
    
    shape.setFillColor(topColor);  // Fallback color
}

void Button::draw(sf::RenderWindow& window) {
    // Draw in order: shadow, main shape, inner glow, text
    window.draw(shadowShape);
    window.draw(mainShape);
    window.draw(innerGlow);
    window.draw(text);
}

bool Button::isMouseOver(const sf::Vector2f& mousePos) const {
    return mainShape.getGlobalBounds().contains(mousePos);
}

void Button::setHighlight(bool highlight) {
    isHovered = highlight;
}