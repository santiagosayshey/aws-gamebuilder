#include "Deck.hpp"
#include <algorithm> // for std::shuffle
#include <random>

Deck::Deck() {
    initialize();
    shuffle();
}

void Deck::initialize() {
    cards.clear();
    // Create a standard 52-card deck: 4 suits, 13 ranks each
    for (int s = 0; s < 4; ++s) {
        Suit suit = static_cast<Suit>(s);
        for (int v = 1; v <= 13; ++v) {
            cards.emplace_back(v, suit);
        }
    }
}

void Deck::shuffle() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(cards.begin(), cards.end(), gen);
}

Card Deck::draw() {
    if (cards.empty()) {
        // If somehow empty, re-init
        initialize();
        shuffle();
    }
    Card top = cards.back();
    cards.pop_back();
    return top;
}

void Deck::reset() {
    initialize();
    shuffle();
}

size_t Deck::size() const {
    return cards.size();
}

const Card& Deck::peek() const {
    return cards.back();
}
