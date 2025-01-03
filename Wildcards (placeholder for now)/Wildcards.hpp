#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "entity/Player.hpp"
#include "entity/Card.hpp"

class Wildcard {
    public:
        virtual ~Wildcard() = default;
        virtual void activate(Player& player, std::vector<std::shared_ptr<Player>>& players, Deck& deck) = 0;
        virtual std::string description() const = 0;
};