#include "Deck.hpp"

Deck::Deck() { initialize(); }

void Deck::initialize() {
  cards.clear();
  for (int suit = 0; suit < 4; ++suit) {
    // Add Aces (value = 1)
    cards.emplace_back(1, static_cast<Suit>(suit));

    // Add number cards (2–10)
    for (int value = 2; value <= 10; ++value) {
      cards.emplace_back(value, static_cast<Suit>(suit));
    }

    // Add face cards (value = 11, 12, 13)
    for (int face = 11; face <= 13; ++face) {
      cards.emplace_back(face, static_cast<Suit>(suit));
    }
  }
}

void Deck::shuffle() {
  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(cards.begin(), cards.end(), g);
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

size_t Deck::size() const { return cards.size(); }

const Card &Deck::peek() const { return cards.back(); }