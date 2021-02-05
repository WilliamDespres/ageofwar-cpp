#include "units/cave/Soldier.hpp"

Soldier::Soldier(int position) : Unit::Unit(stats["CaveAge"]["Soldier"].price, stats["CaveAge"]["Soldier"].hp, stats["CaveAge"]["Soldier"].atk, position, std::make_pair(stats["CaveAge"]["Soldier"].range_1, stats["CaveAge"]["Soldier"].range_2)) {}

std::map<int, std::map<bool, std::map<bool, std::string> > > Soldier::m_normalSkin = {};
std::map<int, std::map<bool, std::map<bool, std::string> > > Soldier::m_attackSkin = {};

std::string Soldier::skin(int h, bool owner) {
  if (m_hasAttacked) { // Then animate attack
    if (h == 3) m_hasAttacked = false; // After animation, we can set it false
    return m_attackSkin[h][owner][m_super];
  } else
    return m_normalSkin[h][owner][m_super];
}

void Soldier::attack(Unit* u) {
  bool hasKilled = u->takeDamage(m_atk);
  if (hasKilled) upgrade(); // Soldier becomes super-soldier if enemy was killed
}

int Soldier::action1(std::vector<std::pair<Unit*, bool>> targets, bool enemyBaseReachable) {
  m_hasDoneAction1 = true;
  m_hasAttacked = true;
  if (targets.size() == 1 && !targets[0].second) { // If there's an unit and it's not an ally
    attack(targets[0].first);
    return 0;
  } else if (enemyBaseReachable)
    return m_atk; // To retrieve hp to enemy base if no enemy has been attacked
  
  m_hasDoneAction1 = false;
  m_hasAttacked = false;
  return 0;
}

void Soldier::action2(int newPosition) {
  Unit::move(newPosition);
}

int Soldier::action3(std::tuple<Unit*, int, bool> target, bool enemyBaseReachable) {
  m_hasAttacked = false;
  if ((!m_hasDoneAction1 || m_super) && std::get<0>(target) && !std::get<2>(target)) { // Can attack on action 3 if it's a super soldier or a normal soldier that hasn't attacked on action 1 and there's an available enemy target
    attack(std::get<0>(target));
    m_hasAttacked = true;
  } else if ((!m_hasDoneAction1 || m_super) && enemyBaseReachable) {
    m_hasAttacked = true;
    return m_atk;
  }
  return 0;
}

void Soldier::initSkin() {
  std::ifstream file("config/skins/cave/soldierSkins.txt");
  std::string line, flag = "";
  int h;
  bool side = true, super = false;
  while (getline(file, line)) {
    if (line == "normal" || line == "attack") {
      h = 0;
      flag = line;
    } else if (line == "change side") {
      side = false;
      super = false;
    } else if (line == "super") {
      super = true;
    } else if (flag == "normal") {
      m_normalSkin[h][side][super] = line;
      h++;
    } else {
      m_attackSkin[h][side][super] = line;
      h++;
    }
  }
}