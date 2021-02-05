#ifndef ROCKETMAN_H
#define ROCKETMAN_H
#include "../Unit.hpp"

class RocketMan : public Unit {
  public:
    RocketMan(int position);
    ~RocketMan() {};

    static std::map<int, std::map<bool, std::string> > m_normalSkin;
    static std::map<int, std::map<bool, std::string> > m_attackSkin;

    // Getters
    std::string name() const { return "R"; }
    std::string skin(int h, bool owner);

    // Setters
    void attack(Unit* u);
    int action1(std::vector<std::pair<Unit*, bool>> targets, bool enemyBaseReachable);
    void action2(int newPosition);
    int action3(std::tuple<Unit*, int, bool> target, bool enemyBaseReachable);
    static void initSkin();
};

#endif