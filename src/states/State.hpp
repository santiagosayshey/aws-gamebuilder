#pragma once
#include <SFML/Graphics.hpp>

enum class StateChange {
    None,
    Menu,
    Settings,
    Help,
    Game,
    Pause,
    Betting
};

class State {
public:
    State(sf::RenderWindow& window) : window(window) {}
    virtual ~State() = default;
    
    virtual void handleInput() = 0;
    virtual void update() = 0;
    virtual void render() = 0;

    StateChange getStateChange() const { return requestedState; }

protected:
    void requestStateChange(StateChange state) { requestedState = state; }
    sf::RenderWindow& window;
    StateChange requestedState = StateChange::None;
};