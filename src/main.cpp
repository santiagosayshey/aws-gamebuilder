#include <SFML/Graphics.hpp>

int main() {
    // Create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Game");

    // Create a circle
    sf::CircleShape circle(50.f);
    circle.setFillColor(sf::Color::Green);
    circle.setPosition(100.f, 100.f);

    // Create a rectangle
    sf::RectangleShape rectangle(sf::Vector2f(120.f, 60.f));
    rectangle.setFillColor(sf::Color::Blue);
    rectangle.setPosition(400.f, 300.f);

    // Create a triangle
    sf::CircleShape triangle(60.f, 3);
    triangle.setFillColor(sf::Color::Red);
    triangle.setPosition(600.f, 100.f);

    // Movement speed
    float speed = 5.0f;

    // Game loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            // Close window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Move circle with arrow keys
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            circle.move(-speed, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            circle.move(speed, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            circle.move(0, -speed);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            circle.move(0, speed);

        // Clear screen
        window.clear(sf::Color::Black);

        // Draw shapes
        window.draw(circle);
        window.draw(rectangle);
        window.draw(triangle);

        // Display what was drawn
        window.display();
    }

    return 0;
}