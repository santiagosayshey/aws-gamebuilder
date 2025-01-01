#pragma once
#include <string>
#include <vector>
#include <memory>

using namespace std;

class Deck; // Wildcard needs object "deck" interaction
class Player; // Player class inherit wildcard object

class Wildcard(){
    public:
    virtual ~Wildcard() = default;
    // The wildcard logic
    virtual void active(Player& player, vector<sharedPtr<Player>>& players, Deck& deck);
    // Description for the card
    virtual string description() const = 0;
}