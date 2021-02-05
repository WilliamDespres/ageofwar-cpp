#include "units/star/Starfighter.hpp"

Starfighter::Starfighter(int position) : Unit::Unit(stats["StarAge"]["Starfighter"].price, stats["StarAge"]["Starfighter"].hp, stats["StarAge"]["Starfighter"].atk, position, std::make_pair(stats["StarAge"]["Starfighter"].range_1, stats["StarAge"]["Starfighter"].range_2)) {}

std::map<int, std::string> Starfighter::m_normalSkin = {};
std::map<int, std::string> Starfighter::m_attackSkin = {};

std::string Starfighter::skin(int h, bool owner) {
  (void) owner;
  if (m_hasAttacked) { // Then animate attack
    if (h == 3) m_hasAttacked = false; // After animation, we can set it false
    return m_attackSkin[h];
  } else
    return m_normalSkin[h];
}

void Starfighter::attack(Unit* u) {
  u->takeDamage(m_atk);
}

int Starfighter::action1(std::vector<std::pair<Unit*, bool>> targets, bool enemyBaseReachable) {
m_hasDoneAction1 = true;
  m_hasAttacked = true;
  int distance;
  for (size_t i = 0; i < targets.size(); i++) // Targets can be at range between 2 and 4 cells
    if (!targets[i].second) { // If it's not an ally
      distance = abs(targets[i].first->getPosition() - m_position);
      attack(targets[i].first);
      if (distance == 4) { // Then catapult attacks cells 3 and 4, but, if nearest enemy is at distance 4, it means there's no enemy at distance 3, maybe it will shoot on a friendly unit
        if (i > 0 && abs(targets[i - 1].first->getPosition() - m_position) == distance - 1) // If there's an unit nearer and it's one cell nearer
          attack(targets[i - 1].first);
      } else { // Attack nearest enemy between range 2 and 3
        if (i < targets.size() - 1 && abs(targets[i + 1].first->getPosition() - m_position) == distance + 1) // If there's an unit one cell further
          attack(targets[i + 1].first);
        else if (enemyBaseReachable && abs(MAP_SIZE - 1 - m_position) == distance + 1) // If there's no other unit to shoot at, enemy base is reachable and its one cell further then it will damage enemy base
          return m_atk;
      }
      return 0; // Not attacking base
    }
  if (enemyBaseReachable) // If none enemy has been attacked and enemy base is reachable, attack it
    return m_atk;

  m_hasDoneAction1 = false;
  m_hasAttacked = false;
  return 0;
}

void Starfighter::action2(int newPosition) {  // Catapult has no action2
  (void) newPosition; // To specify it is unused and avoid warning
}

/*
  Target contains unit located at next cell and its position
*/
int Starfighter::action3(std::tuple<Unit*, int, bool> target, bool enemyBaseReachable) {
  (void) enemyBaseReachable; // To specify it is unused and avoid warning
  if (!m_hasDoneAction1 && !std::get<0>(target))
    Unit::move(std::get<1>(target));
  return 0; // It doesn't attack enemy base in anyway
}

void Starfighter::initSkin() {
  std::ifstream file("config/skins/star/starfighterSkins.txt");
  std::string line, flag = "";
  int h;
  while (getline(file, line)) {
    if (line == "normal" || line == "attack") {
      h = 0;
      flag = line;
    } else if (flag == "normal") {
      m_normalSkin[h] = line;
      h++;
    } else {
      m_attackSkin[h] = line;
      h++;
    }
  }
}