#pragma once
#include <vector>
#include <memory>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include "Card.hpp"
#include "./entity/wildcard/Wildcard.hpp"

class Deck
{
public:
    Deck() void shuffle();
    Card draw();
    void reset();
    size_t size() const;

private:
    std::vector<Card> cards;
    void initialize();
};