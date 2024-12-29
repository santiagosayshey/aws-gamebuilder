#include "Dice.hpp"

Dice::Dice(){}

int Dice::rollDice{
    srand(static_cast<unsigned>(time(0)));
    return random_number = (rand() % 6) + 1;
}
