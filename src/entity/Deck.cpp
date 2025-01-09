#include "Deck.hpp"

Deck::Deck()
{
    initialize();
}

void Deck::initialize()
{
    cards.clear();
    for (int i = 0; i < 4; ++i)
    {
        for (int suit = 0; suit < 4; ++suit)
        {
            for (int value = 1; value <= 13; ++value)
            {
                cards.emplace_back(value, static_cast<Suit>(suit));
            }
        }
    }
}

void Deck::shuffle()
{
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(cards.begin(), cards.end(), g);
}

Card Deck::draw()
{
    if (cards.empty())
    {
        initialize();
        shuffle();
    }

    Card card = cards.back();
    cards.pop_back();
    return card;
}

void Deck::reset()
{
    initialize();
    shuffle();
}

size_t Deck::size() const
{
    return cards.size();
}

const Card &Deck::peek() const
{
    return cards.back();
}