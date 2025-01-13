#pragma once

#include <vector>
#include "../entity/Card.hpp"

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
