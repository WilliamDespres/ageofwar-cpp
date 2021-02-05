#ifndef AI_H
#define AI_H
#include "Player.hpp"

class AI : public Player {
  public:
    AI(std::string name, std::string age, int hp, int money, int xp, bool side);
    ~AI() {};

    UnitNames makeDecision(char key);
    std::string toString() const { return "  AI"; };
};

#endif