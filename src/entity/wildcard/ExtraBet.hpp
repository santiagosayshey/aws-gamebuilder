#pragma once

#include "Wildcard.hpp"
#include "Player.hpp"
#include <string>
#include <vector>

class ExtraBet : public Wildcard {
public:
    ExtraBet();
    virtual ~ExtraBet();
    virtual void use(Player& owner, std::vector<Player>& allPlayers) override;
    virtual std::string getName() const override;
    virtual std::string getDescription() const override;
    virtual Timing getUsageTiming() const override;
};
