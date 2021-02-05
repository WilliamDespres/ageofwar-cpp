#include "players/Human.hpp"

Human::Human(std::string name, std::string age, int hp, int money, int xp, bool side) : Player::Player(name, age, hp, money, xp, side) {}

/*
  This simple AI tries to spend money one biggest units first and spawn it directly
*/
UnitNames Human::makeDecision(char key) {
  if (key != '\0') {
    std::map<char, UnitNames> keys_p1 = { {'q', m_age == "CaveAge" ? UnitNames::Soldier : (m_age == "ModernAge" ? UnitNames::Trooper : UnitNames::R2D2)}, {'s', m_age == "CaveAge" ? UnitNames::Bowman : (m_age == "ModernAge" ? UnitNames::RocketMan : UnitNames::Yoda)}, {'d', m_age == "CaveAge" ? UnitNames::Catapult : (m_age == "ModernAge" ? UnitNames::Tank : UnitNames::Starfighter)} };
    std::map<char, UnitNames> keys_p2 = { {'k', m_age == "CaveAge" ? UnitNames::Soldier : (m_age == "ModernAge" ? UnitNames::Trooper : UnitNames::R2D2)}, {'l', m_age == "CaveAge" ? UnitNames::Bowman : (m_age == "ModernAge" ? UnitNames::RocketMan : UnitNames::Yoda)}, {'m', m_age == "CaveAge" ? UnitNames::Catapult : (m_age == "ModernAge" ? UnitNames::Tank : UnitNames::Starfighter)} };
    std::map<char, UnitNames> keys;
    UnitNames u = UnitNames::None;

    if (!m_side) keys = keys_p1;
    else         keys = keys_p2;

    if (keys.find(key) != keys.end()) {
      u = keys[key];
      if (getMoney() >= Unit::getPrice(u))
        return u;
    }
  }
  return UnitNames::None;
}