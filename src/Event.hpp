#ifndef GAMEEVENT_H
#define GAMEEVENT_H

#include <string>
#include <vector>
#include "Player.cpp"

class GameEvent{    
    public:
    // event handler should impact salary and happiness depending on the cards
        struct GameEvent
        {
            std::string eventDescription; // event description
            int salaryEffect; // Effect on the player's salary
            int happinessEffect; // Effect on the player's happiness
            bool isPlayerImpacted; // Does this impact the player?
        };
    
    GameEvent()
    // list the necessary functions
    void goodEvent(Player& player);
    void badEvent(Player& player);
    void impactPlayers(Player& player, Player& target);
    void drawChance(Player& player);

    private:
    
}

