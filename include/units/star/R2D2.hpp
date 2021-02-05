#ifndef R2D2_H
#define R2D2_H
#include "../Unit.hpp"

class R2D2 : public Unit {
  private:
    bool m_hasDoneAction1 = false; // Keep track if it has done it's action 1 this turn

  public:
    R2D2(int position);
    ~R2D2() {};

    static std::map<int, std::map<bool, std::string> > m_normalSkin;
    static std::map<int, std::map<bool, std::string> > m_attackSkin;

    // Getters
    std::string name() const { return "R2"; }
    std::string skin(int h, bool owner);

    // Setters
    void attack(Unit* u);
    int action1(std::vector<std::pair<Unit*, bool>> targets, bool enemyBaseReachable);
    void action2(int newPosition);
    int action3(std::tuple<Unit*, int, bool> target, bool enemyBaseReachable);
    static void initSkin();
};

#endif