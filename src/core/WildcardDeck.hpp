#pragma once

#include <vector>
#include <memory>
#include "../entity/wildcard/Wildcard.hpp"

class WildcardDeck {
public:
    WildcardDeck();
    void shuffle();
    std::shared_ptr<Wildcard> draw();
    void reset();

private:
    std::vector<std::shared_ptr<Wildcard>> cards;
    void initialize();
};
