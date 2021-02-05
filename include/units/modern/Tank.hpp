#ifndef TANK_H
#define TANK_H
#include "../Unit.hpp"

class Tank : public Unit {
  private:
    bool m_hasDoneAction1 = false; // Keep track if it has done it's action 1 this turn

  public:
    Tank(int position);
    ~Tank() {};

    static std::map<int, std::map<bool, std::string> > m_normalSkin;
    static std::map<int, std::map<bool, std::string> > m_attackSkin;

    // Getters
    std::string name() const { return "TA"; }
    std::string skin(int h, bool owner);

    // Setters
    void attack(Unit* u);
    int action1(std::vector<std::pair<Unit*, bool>> targets, bool enemyBaseReachable);
    void action2(int newPosition);
    int action3(std::tuple<Unit*, int, bool> target, bool enemyBaseReachable);
    static void initSkin();
};

#endif