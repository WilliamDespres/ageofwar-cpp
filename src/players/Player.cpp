#include "players/Player.hpp"

Player::Player(std::string name, std::string age, int hp, int money, int xp, bool side) : m_name(name), m_age(age), m_hp(hp), m_money(money), m_xp(xp), m_side(side) {}

std::map<std::string, std::map<bool, std::map<int, std::map<int, std::string> > > > Player::m_skins = {};

void Player::takeDamage(int dmg) {
  m_hp -= dmg;
}

void Player::spawn(UnitNames unit) {
  int position = 0;
  if (m_side) // If I'm player2 (right side)
    position = MAP_SIZE - 1;

  if (!getNearestUnit() || getNearestUnit()->getPosition() != position) { // Avoid creating an unit if there's one at spawn
    switch (unit) {
      case UnitNames::Soldier:
        m_units.push_back(new Soldier(position));
        break;
      case UnitNames::Bowman:
        m_units.push_back(new Bowman(position));
        break;
      case UnitNames::Catapult:
        m_units.push_back(new Catapult(position));
        break;
      case UnitNames::Trooper:
        m_units.push_back(new Trooper(position));
        break;
      case UnitNames::RocketMan:
        m_units.push_back(new RocketMan(position));
        break;
      case UnitNames::Tank:
        m_units.push_back(new Tank(position));
        break;
      case UnitNames::R2D2:
        m_units.push_back(new R2D2(position));
        break;
      case UnitNames::Yoda:
        m_units.push_back(new Yoda(position));
        break;
      case UnitNames::Starfighter:
        m_units.push_back(new Starfighter(position));
        break;
      default:
        break;
    }
    pay(Unit::getPrice(unit));
  }
}

void Player::spawn(UnitNames unit, int hp, int pos) {
  Unit* u = nullptr;
  switch (unit) {
    case UnitNames::Soldier:
      u = new Soldier(pos);
      break;
    case UnitNames::SuperSoldier: {
      u = new Soldier(pos);
      Soldier* s = new Soldier(pos);
      s->takeDamage(s->getHp()); // Set hp to 0
      u->attack(s); // Kill it so it becomes super soldier
      delete s;
      break;
    }
    case UnitNames::Bowman:
      u = new Bowman(pos);
      break;
    case UnitNames::Catapult:
      u = new Catapult(pos);
      break;
    case UnitNames::Trooper:
      m_units.push_back(new Trooper(pos));
      break;
    case UnitNames::RocketMan:
      m_units.push_back(new RocketMan(pos));
      break;
    case UnitNames::Tank:
      m_units.push_back(new Tank(pos));
      break;
    case UnitNames::R2D2:
      m_units.push_back(new R2D2(pos));
      break;
    case UnitNames::Yoda:
      m_units.push_back(new Yoda(pos));
      break;
    case UnitNames::Starfighter:
      m_units.push_back(new Starfighter(pos));
      break;
    default:
      break;
  }
  if (u) {
    u->takeDamage(u->getHp() - hp);
    m_units.push_back(u);
  }
}

void Player::upgradeAge() {
  if (m_xp == 100) {
    if (m_age == "CaveAge") {
      m_age = "ModernAge";
      m_xp = 0;
      return;
    }
    else if (m_age == "ModernAge") {
      m_age = "StarAge";
      m_xp = 0;
      return;
    }
  }
}

/*
  Because new spawned units are pushed at the back of m_units, we know for sure the bigger the index the closer the unit
  So farest unit is first index
*/
Unit* Player::getFarestUnit() const {
  if (m_units.size() > 0)
    return m_units[0];
  return nullptr;
}

Unit* Player::getNearestUnit() const {
  if (m_units.size() > 0)
    return m_units[m_units.size() - 1];
  return nullptr;
}

Unit* Player::getUnitAtPosition(int position) const {
  for (Unit* u : m_units)
    if (u->getPosition() == position)
      return u;
  return nullptr;
}

std::string Player::skin(int h) {
  if (m_hp > 75)
    return m_skins[m_age][m_side][h][100];
  else if (m_hp > 50)
    return m_skins[m_age][m_side][h][75];
  else if (m_hp > 25)
    return m_skins[m_age][m_side][h][50];
  else
    return m_skins[m_age][m_side][h][25];
}

// Returns the reward gained from all dead units
int Player::killAllDeadUnits() {
  int reward = 0;
  for (size_t i = 0; i < m_units.size(); i++)
    if (m_units[i]->getHp() < 1) {
      reward += m_units[i]->getPrice() / 2;
      delete m_units[i]; // Destroy object
      m_units.erase(m_units.begin() + i); // Delete index from vector
      i--; // We go 1 element backwards in units due to deletion of the i-th element.
    }
  if (m_age != "StarAge") upgradeAge();
  return reward;
}

void Player::initSkin() {
  std::ifstream file("config/skins/castleSkins.txt");
  std::string line, flag = "";
  int h, hp;
  bool side;
  while (getline(file, line)) {
    if (line == "CaveAge" || line == "ModernAge" || line == "StarAge") {
      h = 0;
      flag = line;
    } else if (line == "P1" || line == "P2") {
      side = line == "P2";
      h = 0;
    } else if (line == "100" || line == "75" || line == "50" || line == "25") {
      hp = std::stoi(line);
      h = 0;
    } else {
      m_skins[flag][side][h][hp] = line;
      h++;
    }
  }
}