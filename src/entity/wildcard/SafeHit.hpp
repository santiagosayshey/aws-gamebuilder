#pragma once

#include "Wildcard.hpp"
#include "Player.hpp"
#include <string>
#include <vector>

class SafeHit : public Wildcard {
public:
    SafeHit::SafeHit() {}
    SafeHit::~SafeHit() {}
    virtual void use(Player& owner, std::vector<Player>& allPlayers) override;
    virtual std::string getName() const override;
    virtual std::string getDescription() const override;
    virtual Timing getUsageTiming() const override;
};
