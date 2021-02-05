#ifndef UNIT_H
#define UNIT_H
#include <string>
#include <vector>
#include <tuple>
#include <map>
#include <fstream>
#include <sstream>
#include <iostream>

#define MAP_SIZE 12

enum class UnitNames { Soldier, Bowman, Catapult, SuperSoldier, Trooper, RocketMan, Tank, R2D2, Yoda, Starfighter, None };
struct UnitStats { int price, hp, atk, range_1, range_2; };

class Unit {
  protected:
    int m_price, m_hp, m_atk, m_position; // Unit price, hit points, attack values and position
    std::pair<int,int> m_range; // According to unit type, range varies between 1 and 4. It can be represented by two bounds
    bool m_hasAttacked = false; // Boolean to keep track if an unit attacks or not on last action, it's updated in each action, used to animate characters
    Unit(int price, int hp, int atk, int position, std::pair<int,int> range);

  public:
    static std::map<std::string, std::map<std::string, UnitStats> > stats; // Associates, to each age, a map that associates stats to each Unit.

    virtual void attack(Unit* u) = 0;
    virtual ~Unit() {};

    // Getters
    int getPrice() const                { return m_price; };
    int getHp() const                   { return m_hp; };
    int getAtk() const                  { return m_atk; };
    int getPosition() const             { return m_position; };
    std::pair<int,int> getRange() const { return m_range; };
    static int getPrice(UnitNames u);
    virtual std::string name() const = 0;
    virtual std::string skin(int h, bool owner) = 0;

    // Setters
    void move(int newPosition) { if (newPosition != 0 && newPosition != MAP_SIZE - 1) m_position = newPosition; }; // Impossible to go on other bases
    bool takeDamage(int dmg); // Returns true if unit is killed, false otherwise
    virtual int action1(std::vector<std::pair<Unit*, bool>> targets, bool enemyBaseReachable) = 0; // return int is used to retrieve hp to enemy base if it has been attacked
    virtual void action2(int newPosition) = 0;
    virtual int action3(std::tuple<Unit*, int, bool> target, bool enemyBaseReachable) = 0; // int is target's position and bool is true if unit is friendly, return int for same reason as action1 does
    static void initStats();
};

#endif