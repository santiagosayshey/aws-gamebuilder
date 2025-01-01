#pragma once
#include <string>
#include <vector>
#include <memory>

using namespace std;

class deck; // wildcard needs object "deck" interaction
class player; // player class inherit wildcard object

class Wildcard(){
    public:
    virtual ~Wildcard() = default;
    // the wildcard logic
    virtual void 
    // description for the card
    virtual string description() const = 0;
}