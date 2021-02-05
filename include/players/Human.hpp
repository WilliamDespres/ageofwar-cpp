#ifndef HUMAN_H
#define HUMAN_H
#include "Player.hpp"

class Human : public Player {
  public:
    Human(std::string name, std::string age, int hp, int money, int xp, bool side);
    ~Human() {};
  
    UnitNames makeDecision(char key);
    std::string toString() const { return "  P "; };
};

#endif