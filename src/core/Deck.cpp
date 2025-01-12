#include "Deck.hpp"
#include <algorithm>
#include <random>

Deck::Deck() {
    initialize();
}

void Deck::initialize() {
    cards.clear();
    // Example of a simple “1–10” deck with 4 suits:
    for (int suit = 0; suit < 4; ++suit) {
        for (int value = 1; value <= 10; ++value) {
            // We store Card( value ), ignoring suit if your Card struct 
            // doesn't handle suits yet. Or you can store an enum Suit if needed.
            cards.emplace_back(value);
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
        initialize();
        shuffle();
    }
    Card card = cards.back();
    cards.pop_back();
    return card;
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
