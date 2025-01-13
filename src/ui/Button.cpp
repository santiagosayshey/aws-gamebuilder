#include "Button.hpp"
#include <algorithm> // For std::min/std::max

Button::Button(const sf::Vector2f& position, 
               const sf::Vector2f& size, 
               const std::string& buttonText, 
               const sf::Font& font,
               const sf::Color& defaultColor,
               const sf::Color& hoverColor)
    : hoverTransition(0.0f)
    , isHovered(false)
{
    mainShape.setSize(size);
    mainShape.setPosition(position);
    
    baseColor = sf::Color(20, 80, 20);
    highlightColor = sf::Color(30, 120, 30);
    currentColor = baseColor;
    shadowColor = sf::Color(0, 20, 0, 180);
    textDefaultColor = defaultColor;
    textHoverColor = hoverColor;
    
    shadowShape.setSize(size);
    shadowShape.setPosition(position + sf::Vector2f(4, 4));
    shadowShape.setFillColor(shadowColor);
    
    innerGlow.setSize(sf::Vector2f(size.x - 4, size.y - 4));
    innerGlow.setPosition(position + sf::Vector2f(2, 2));
    
    // Setup text
    text.setFont(font);
    text.setString(buttonText);
    text.setCharacterSize(30);
    text.setFillColor(textDefaultColor);
    text.setStyle(sf::Text::Bold);
    
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin(textBounds.width / 2.f, textBounds.height / 2.f);
    text.setPosition(
        position.x + size.x / 2.f,
        position.y + size.y / 2.f - 2.f
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
    // Interpolate between baseColor and highlightColor
    currentColor = sf::Color(
        baseColor.r + static_cast<sf::Uint8>((highlightColor.r - baseColor.r) * hoverTransition),
        baseColor.g + static_cast<sf::Uint8>((highlightColor.g - baseColor.g) * hoverTransition),
        baseColor.b + static_cast<sf::Uint8>((highlightColor.b - baseColor.b) * hoverTransition)
    );
    
    // Create gradient on mainShape
    createGradient(
        mainShape, 
        currentColor, 
        sf::Color(currentColor.r / 2, currentColor.g / 2, currentColor.b / 2)
    );
    
    // Update innerGlow (white gradient)
    sf::Color glowColor(
        255, 
        255, 
        255, 
        static_cast<sf::Uint8>(40 + 40 * hoverTransition)
    );
    createGradient(innerGlow, glowColor, sf::Color(255, 255, 255, 0));
    
    // Update text color
    text.setFillColor(sf::Color(
        textDefaultColor.r + static_cast<sf::Uint8>((textHoverColor.r - textDefaultColor.r) * hoverTransition),
        textDefaultColor.g + static_cast<sf::Uint8>((textHoverColor.g - textDefaultColor.g) * hoverTransition),
        textDefaultColor.b + static_cast<sf::Uint8>((textHoverColor.b - textDefaultColor.b) * hoverTransition)
    ));
}

void Button::createGradient(sf::RectangleShape& shape, 
                            const sf::Color& topColor, 
                            const sf::Color& bottomColor) 
{
    // This function sets a fill color as a fallback.
    // SFML doesn't natively do gradient fills for shapes, so a typical approach is 
    // to use a sf::VertexArray if you want a real gradient. 
    // For now, we just store topColor so it's not purely flat green:
    shape.setFillColor(topColor);
}

void Button::draw(sf::RenderWindow& window) {
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

// NEW: Provide public access to the button's top-left corner position
sf::Vector2f Button::getPosition() const {
    return mainShape.getPosition();
}
