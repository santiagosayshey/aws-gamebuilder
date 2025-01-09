/*
#include "Wildcard.hpp"
#include "entity/Player.hpp"

class InstantStand : public Wildcard
{
public:
    InstantStand() = default;
    ~InstantStand() = default;

    void use(Player &owner, std::vector<Player> &allPlayers) override
    {
        player.setTotal(20)
    }

    std::string getName() const override
    {
        return "Instant Stand";
    }

    std::string getDescription() const override
    {
        return "Sets your hand total to 20.";
    }

    Timing getUsageTiming() const override
    {
        return Timing::ANY_TIME;
    }
};
*/