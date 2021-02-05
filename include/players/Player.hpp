#ifndef PLAYER_H
#define PLAYER_H
#include <vector>
#include <iomanip>
#include <chrono>
#include <thread>
#include <map>

#include "units/Unit.hpp"
#include "units/cave/Soldier.hpp"
#include "units/cave/Catapult.hpp"
#include "units/cave/Bowman.hpp"
#include "units/modern/Trooper.hpp"
#include "units/modern/RocketMan.hpp"
#include "units/modern/Tank.hpp"
#include "units/star/R2D2.hpp"
#include "units/star/Yoda.hpp"
#include "units/star/Starfighter.hpp"

class Player {
  protected:
    std::vector<Unit*> m_units;
    std::string m_name, m_age; // m_age is either CaveAge or ModernAge
    int m_hp, m_money, m_xp;
    bool m_side; // Player1 is left so m_side = false, Player2 is right so m_side = true
    Player(std::string name, std::string age, int hp, int money, int xp, bool side);

  public:
    virtual ~Player() { m_units.clear(); m_units.shrink_to_fit(); };

    static std::map<std::string, std::map<bool, std::map<int, std::map<int, std::string> > > > m_skins; // First key is age, second key is player, third key is height and last key is hp

    // Getters
    std::string getName() const { return m_name; };
    std::string getAge() const { return m_age; };
    int getHp() const { return m_hp; };
    int getMoney() const { return m_money; };
    int getXp() const { return m_xp; };
    std::vector<Unit*> getUnits() const { return m_units; };
    Unit* getFarestUnit() const;
    Unit* getNearestUnit() const;
    Unit* getUnitAtPosition(int position) const;
    std::string skin(int h);
    
    // Setters
    void takeDamage(int dmg);
    void spawn(UnitNames unit);
    void spawn(UnitNames unit, int hp, int pos);
    void upgradeAge();
    int killAllDeadUnits(); // Returns the reward gained from all dead units
    void pay(int amount) { m_money -= amount; };
    void receiveMoney(int amount) { m_money += amount; };
    void receiveXp(int amount) { m_xp = std::min(m_xp + amount, 100); };
    static void initSkin();

    virtual UnitNames makeDecision(char key) = 0; // Pure virtual
    virtual std::string toString() const = 0; // Pure virtual
};

#endif