#ifndef SOLDIER_H
#define SOLDIER_H
#include "../Unit.hpp"

class Soldier : public Unit {
  private:
    bool m_super = false; // Super soldier, false by default
    bool m_hasDoneAction1 = false; // Keep track if it has done it's action 1 this turn
    void upgrade() { m_super = true; };

  public:
    Soldier(int position);
    ~Soldier() {};

    static std::map<int, std::map<bool, std::map<bool, std::string> > > m_normalSkin;
    static std::map<int, std::map<bool, std::map<bool, std::string> > > m_attackSkin;

    // Getters
    bool isSuper() const { return m_super; }
    std::string name() const { return m_super ? "S" : "s"; }
    std::string skin(int h, bool owner);

    // Setters
    void attack(Unit* u);
    int action1(std::vector<std::pair<Unit*, bool>> targets, bool enemyBaseReachable);
    void action2(int newPosition);
    int action3(std::tuple<Unit*, int, bool> target, bool enemyBaseReachable);
    static void initSkin();
};

#endif