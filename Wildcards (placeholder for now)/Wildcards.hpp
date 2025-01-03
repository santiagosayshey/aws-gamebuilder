#pragma once
#include <iostream>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
//#include "/entity/player.hpp"
//#include "/entity/card.hpp"
#include "states/GameState.hpp"


using namespace std;

// specify the wildcard types
enum class WildcardType{
    Swap,
    Foresight,
    SeeThrough,
    Duplicate,
    DeckPeek,
    ExtraBet,
    SafeHit,
    InstantStand,
    Freeze,
    SecondChance
};

class Wildcard{
    public:
        virtual ~Wildcard() = default;
        // the wildcard logic
        virtual void activate(Player& player, std::vector<std::shared_ptr<Player>>& players, Deck& deck) = 0;
        // description
        virtual std::string description() const = 0;
        // Factory method to create instance of wildcard
        static std::shared_ptr<Wildcard> createWildcard(WildcardType type);
};