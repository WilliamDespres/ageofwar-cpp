#ifndef YODA_H
#define YODA_H
#include "../Unit.hpp"

class Yoda : public Unit {
  public:
    Yoda(int position);
    ~Yoda() {};

    static std::map<int, std::map<bool, std::string> > m_normalSkin;
    static std::map<int, std::map<bool, std::string> > m_attackSkin;

    // Getters
    std::string name() const { return "Y"; }
    std::string skin(int h, bool owner);

    // Setters
    void attack(Unit* u);
    int action1(std::vector<std::pair<Unit*, bool>> targets, bool enemyBaseReachable);
    void action2(int newPosition);
    int action3(std::tuple<Unit*, int, bool> target, bool enemyBaseReachable);
    static void initSkin();
};

#endif