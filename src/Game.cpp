// Game.cpp
#include "Game.hpp"

Game::Game() : window(sf::VideoMode(800, 600), "Money != Happiness") {
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
    }
}

void Game::update() {
}

void Game::render() {
    window.clear();
    window.display();
}

void Game::run() {
    while (window.isOpen()) {
        processEvents();
        update();
        render();
    }
}
