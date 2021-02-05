#include "units/star/R2D2.hpp"

R2D2::R2D2(int position) : Unit::Unit(stats["StarAge"]["R2D2"].price, stats["StarAge"]["R2D2"].hp, stats["StarAge"]["R2D2"].atk, position, std::make_pair(stats["StarAge"]["R2D2"].range_1, stats["StarAge"]["R2D2"].range_2)) {}

std::map<int, std::map<bool, std::string> > R2D2::m_normalSkin = {};
std::map<int, std::map<bool, std::string> > R2D2::m_attackSkin = {};

std::string R2D2::skin(int h, bool owner) {
  if (m_hasAttacked) { // Then animate attack
    if (h == 3) m_hasAttacked = false; // After animation, we can set it false
    return m_attackSkin[h][owner];
  } else
    return m_normalSkin[h][owner];
}

void R2D2::attack(Unit* u) {
  u->takeDamage(m_atk);
}

int R2D2::action1(std::vector<std::pair<Unit*, bool>> targets, bool enemyBaseReachable) {
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

void R2D2::action2(int newPosition) {
  Unit::move(newPosition);
}

int R2D2::action3(std::tuple<Unit*, int, bool> target, bool enemyBaseReachable) {
  m_hasAttacked = false;
  if (!m_hasDoneAction1 && std::get<0>(target) && !std::get<2>(target)) { // Can attack on action 3 if it's a super soldier or a normal soldier that hasn't attacked on action 1 and there's an available enemy target
    attack(std::get<0>(target));
    m_hasAttacked = true;
  } else if (!m_hasDoneAction1 && enemyBaseReachable) {
    m_hasAttacked = true;
    return m_atk;
  }
  return 0;
}

void R2D2::initSkin() {
  std::ifstream file("config/skins/star/r2d2Skins.txt");
  std::string line, flag = "";
  int h;
  bool side = true;
  while (getline(file, line)) {
    if (line == "normal" || line == "attack") {
      h = 0;
      flag = line;
    } else if (line == "change side") {
      side = false;
    } else if (flag == "normal") {
      m_normalSkin[h][side] = line;
      h++;
    } else {
      m_attackSkin[h][side] = line;
      h++;
    }
  }
}