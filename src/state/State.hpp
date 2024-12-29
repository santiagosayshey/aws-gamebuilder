// State.hpp
#ifndef STATE_HPP
#define STATE_HPP

#include <SFML/Graphics.hpp>

class State {
    public:
        State() = default;
        virtual ~State() = default;

        // must be implemented by derived classes
        virtual void processEvent(sf::Event& event) = 0;
        virtual void update() = 0;
        virtual void draw(sf::RenderWindow& window) = 0;    

};

#endif