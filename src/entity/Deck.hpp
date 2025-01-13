#pragma once
#include <vector>
#include <memory>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include "Card.hpp"

class Deck
{
public:
    Deck();
    void shuffle();
    Card draw();
    void reset();
    size_t size() const;
    const Card &peek() const;

private:
    std::vector<Card> cards;
    void initialize();
};