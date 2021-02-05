#include "units/Unit.hpp"

Unit::Unit(int price, int hp, int atk, int position, std::pair<int, int> range) : m_price(price), m_hp(hp), m_atk(atk), m_position(position), m_range(range) {}

std::map<std::string, std::map<std::string, UnitStats> > Unit::stats = {};

/*
  Returns true if unit is killed, false otherwise
  The return value must be used to destroy unit in Game.cpp
*/
bool Unit::takeDamage(int dmg) { 
  m_hp -= dmg;
  if (m_hp <= 0) return true;
  else return false;
}

int Unit::getPrice(UnitNames u) {
  switch(u) {
    case UnitNames::Soldier:
    case UnitNames::SuperSoldier:
      return stats["CaveAge"]["Soldier"].price;
    case UnitNames::Bowman:
      return stats["CaveAge"]["Bowman"].price;
    case UnitNames::Catapult:
      return stats["CaveAge"]["Catapult"].price;
    case UnitNames::Trooper:
      return stats["ModernAge"]["Trooper"].price;
    case UnitNames::RocketMan:
      return stats["ModernAge"]["RocketMan"].price;
    case UnitNames::Tank:
      return stats["ModernAge"]["Tank"].price;
    case UnitNames::R2D2:
      return stats["StarAge"]["R2D2"].price;
    case UnitNames::Yoda:
      return stats["StarAge"]["Yoda"].price;
    case UnitNames::Starfighter:
      return stats["StarAge"]["Starfighter"].price;
    default:
      return 0;
  }
}

void Unit::initStats() {
  std::ifstream file("config/stats.txt");
  std::string line, flag, age;
  while (getline(file, line)) {
    std::istringstream l(line);
    l >> age;
    l >> flag;
    l >> stats[age][flag].price;
    l >> stats[age][flag].hp;
    l >> stats[age][flag].atk;
    l >> stats[age][flag].range_1;
    l >> stats[age][flag].range_2;
  }
}