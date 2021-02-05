#include "players/AI.hpp"
#include <iostream>

AI::AI(std::string name, std::string age, int hp, int money, int xp, bool side) : Player::Player(name, age, hp, money, xp, side) {}

/*
  This simple AI tries to spend money one biggest units first and spawn it directly
*/
UnitNames AI::makeDecision(char key) {
  (void) key; // Useless for an AI
  UnitNames u = UnitNames::None; // Spawned unit
  if (m_money >= Unit::getPrice(m_age == "CaveAge" ? UnitNames::Catapult : (m_age == "ModernAge" ? UnitNames::Tank : UnitNames::Starfighter))) {
    u = m_age == "CaveAge" ? UnitNames::Catapult : (m_age == "ModernAge" ? UnitNames::Tank : UnitNames::Starfighter);
  } else if (m_money >= Unit::getPrice(m_age == "CaveAge" ? UnitNames::Bowman : (m_age == "ModernAge" ? UnitNames::RocketMan : UnitNames::Yoda))) {
    u = m_age == "CaveAge" ? UnitNames::Bowman : (m_age == "ModernAge" ? UnitNames::RocketMan : UnitNames::Yoda);
  } else if (m_money >= Unit::getPrice(m_age == "CaveAge" ? UnitNames::Soldier : (m_age == "ModernAge" ? UnitNames::Trooper : UnitNames::R2D2))) {
    u = m_age == "CaveAge" ? UnitNames::Soldier : (m_age == "ModernAge" ? UnitNames::Trooper : UnitNames::R2D2);
  }
  return u;
}