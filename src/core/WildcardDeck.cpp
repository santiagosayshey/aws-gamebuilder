#include "WildcardDeck.hpp"
#include <random>
#include <algorithm>

// Include all your wildcard subclasses
#include "../entity/wildcard/ExtraBet.hpp"
#include "../entity/wildcard/Duplicate.hpp"
#include "../entity/wildcard/Tycoon.hpp"
#include "../entity/wildcard/InstantStand.hpp"
#include "../entity/wildcard/SafeHit.hpp"
#include "../entity/wildcard/BalanceShift.hpp"
#include "../entity/wildcard/Foresight.hpp"
// etc.

WildcardDeck::WildcardDeck() {
    initialize();
    shuffle();
}

void WildcardDeck::initialize() {
    cards.clear();
    // Add as many wildcards as you want:
    cards.push_back(std::make_shared<ExtraBet>());
    cards.push_back(std::make_shared<Duplicate>());
    cards.push_back(std::make_shared<Tycoon>());
    cards.push_back(std::make_shared<InstantStand>());
    cards.push_back(std::make_shared<Foresight>());
    cards.push_back(std::make_shared<SafeHit>());
    cards.push_back(std::make_shared<BalanceShift>());

    // ... add more if desired
}

void WildcardDeck::shuffle() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(cards.begin(), cards.end(), gen);
}

std::shared_ptr<Wildcard> WildcardDeck::draw() {
    if (cards.empty()) {
        initialize();
        shuffle();
    }
    auto w = cards.back();
    cards.pop_back();
    return w;
}

void WildcardDeck::reset() {
    initialize();
    shuffle();
}
