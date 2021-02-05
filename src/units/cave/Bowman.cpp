#include "units/cave/Bowman.hpp"

Bowman::Bowman(int position) : Unit::Unit(stats["CaveAge"]["Bowman"].price, stats["CaveAge"]["Bowman"].hp, stats["CaveAge"]["Bowman"].atk, position, std::make_pair(stats["CaveAge"]["Bowman"].range_1, stats["CaveAge"]["Bowman"].range_2)) {}

std::map<int, std::map<bool, std::string> > Bowman::m_normalSkin = {};
std::map<int, std::map<bool, std::string> > Bowman::m_attackSkin = {};

std::string Bowman::skin(int h, bool owner) {
  if (m_hasAttacked) { // Then animate attack
    if (h == 3) m_hasAttacked = false; // After animation, we can set it false
    return m_attackSkin[h][owner];
  } else
    return m_normalSkin[h][owner];
}

void Bowman::attack(Unit* u) {
  u->takeDamage(m_atk);
}

int Bowman::action1(std::vector<std::pair<Unit*, bool>> targets, bool enemyBaseReachable) {
  m_hasAttacked = true;
  for (size_t i = 0; i < targets.size(); i++)
    if (!targets[i].second) { // If it's not an ally
      attack(targets[i].first); // Attack nearest enemy
      return 0;
    }
  if (enemyBaseReachable) // If none enemy has been attacked and enemy base is reachable, attack it
    return m_atk;

  m_hasAttacked = false;
  return 0; 
}

void Bowman::action2(int newPosition) {
  Unit::move(newPosition);
}

int Bowman::action3(std::tuple<Unit*, int, bool> target, bool enemyBaseReachable) { // Bowman has no action3
  (void) target; (void) enemyBaseReachable; // To specify it is unused and avoid warning
  return 0; // It doesn't attack enemy base in anyway
}

void Bowman::initSkin() {
  std::ifstream file("config/skins/cave/bowmanSkins.txt");
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