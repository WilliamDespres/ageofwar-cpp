#ifndef STARFIGHTER_H
#define STARFIGHTER_H
#include "../Unit.hpp"

class Starfighter : public Unit {
  private:
    bool m_hasDoneAction1 = false; // Keep track if it has done it's action 1 this turn

  public:
    Starfighter(int position);
    ~Starfighter() {};

    static std::map<int, std::string> m_normalSkin;
    static std::map<int, std::string> m_attackSkin;

    // Getters
    std::string name() const { return "ST"; }
    std::string skin(int h, bool owner);

    // Setters
    void attack(Unit* u);
    int action1(std::vector<std::pair<Unit*, bool>> targets, bool enemyBaseReachable);
    void action2(int newPosition);
    int action3(std::tuple<Unit*, int, bool> target, bool enemyBaseReachable);
    static void initSkin();
};

#endif