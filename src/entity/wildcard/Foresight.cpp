
#include "Wildcard.hpp"
#include "entity/Player.hpp"
#include "states/GameState.hpp"

class Foresight : public Wildcard
{
public:
    Foresight() = default;
    ~Foresight() = default;

    void use(Player &owner, std::vector<Player> &allPlayers) override
    {
        const Card &nextCard = gameState.peekNextCard();
    }

    std::string getName() const override
    {
        return "Foresight";
    }

    std::string getDescription() const override
    {
        return "Allows you to see the next card in the deck.";
    }

    Timing getUsageTiming() const override
    {
        return Timing::ANY_TIME;
    }

private:
    GameState &gameState;
};
