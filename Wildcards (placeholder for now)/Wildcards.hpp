#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>
//#include "/entity/player.hpp"
//#include "/entity/card.hpp"


using namespace std;

class Wildcard{
    public:
        virtual ~Wildcard() = default;
        // the wildcard logic
        virtual void activate(Player& player, std::vector<std::shared_ptr<Player>>& players, Deck& deck) = 0;
        virtual std::string description() const = 0;
};