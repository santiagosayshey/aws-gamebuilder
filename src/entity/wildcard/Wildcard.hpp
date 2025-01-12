#pragma once

#include <string>
#include <vector>
#include <memory>

class Player;

class Wildcard {
public:
    virtual ~Wildcard() = default;

    virtual void use(Player& owner, std::vector<Player>& allPlayers) = 0;
    virtual std::string getName() const = 0;
    virtual std::string getDescription() const = 0;

    enum class Timing {
        BEFORE_DRAW,
        AFTER_DRAW,
        ANY_TIME,
        AFTER_BUST
    };

    virtual Timing getUsageTiming() const = 0;
};
