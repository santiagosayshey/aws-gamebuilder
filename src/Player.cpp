#include "Dice.hpp"
#include <iostream>
#include <string>
#include <memory>

using namespace std;

class Player {
    private:
    string name;
    int happiness, money;

    public:
    Player(const std::string& playerName, int happinessLevel, int moneyAmount)
        : name(playerName), happiness(happinessLevel), money(moneyAmount) {}
}

