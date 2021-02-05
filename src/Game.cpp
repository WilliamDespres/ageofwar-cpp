#include "Game.hpp"

/*
  Args are used to defined which play-mode.
    - Argc = 1 (none provided) OR Argc > 1 and argv = 0 -> PvP
    - Argc > 1 and argv = 1 -> PvAI
    - Argc > 1 and argv = 2 -> AIvAI
*/
Game::Game(int maxTurnNumber, int argc, char *argv[]) : m_name("Game"), m_turnNumber(1), m_playerTurn(false), m_isSaved(false), m_isLoaded(false), m_isPlaying(true), m_player1Key('\0'), m_player2Key('\0') {
  initAllStatics();
  m_maxTurnNumber = maxTurnNumber;

  if (argc > 1) {
    int mode = std::stoi(argv[1]);
    switch (mode) {
    case 1: // PvAI
      m_player1 = new Human("P1", "CaveAge", 100, 0, 0, false);
      m_player2 = new AI("AI2", "CaveAge", 100, 0, 0, true);
      break;
    case 2: // AIvAI
      m_player1 = new AI("AI1", "CaveAge", 100, 0, 0, false);
      m_player2 = new AI("AI2", "CaveAge", 100, 0, 0, true);
      break;
    default: // PvP (0 or anything else)
      m_player1 = new Human("P1", "CaveAge", 100, 0, 0, false);
      m_player2 = new Human("P2", "CaveAge", 100, 0, 0, true);
      break;
    }
  } else {
    m_player1 = new Human("P1", "CaveAge", 100, 0, 0, false);
    m_player2 = new Human("P2", "CaveAge", 100, 0, 0, true);
  }

  runGame();
}

void Game::initAllStatics() {
  Unit::initStats();
  Soldier::initSkin();
  Bowman::initSkin();
  Catapult::initSkin();
  Trooper::initSkin();
  RocketMan::initSkin();
  Tank::initSkin();
  R2D2::initSkin();
  Yoda::initSkin();
  Starfighter::initSkin();
  Player::initSkin();
}

void Game::runGame() {
  while(m_turnNumber <= m_maxTurnNumber && m_player1->getHp() > 0 && m_player2->getHp() > 0) {
    m_player1->receiveMoney(8); // Each turn, player1 receives 8 coins
    m_player2->receiveMoney(8); // Each turn, player2 receives 8 coins
    
    gameTurn(); // Player1's turn, their basement is at index 0
    
    gameTurn(); // Player2's turn, their basement is at last index
    m_turnNumber++; // Increase turn number      
  }
  // Reasons why game ended
  if (m_player1->getHp() <= 0) std::cout << "P2 has won !" << std::endl;
  else if (m_player2->getHp() <= 0) std::cout << "P1 has won !" << std::endl;
  else std::cout << m_turnNumber - 1 << " turns played: Draw!" << std::endl;
}

/*
  We've decided to check each time unit's hp instead of deleting it when it's dead to avoid problems with indexes
*/
void Game::gameTurn() {
  Player* playingP = getPlayingPlayer();
  Player* enemyPlayer = getWaitingPlayer();
  int direction = (m_playerTurn ? -1 : 1); // According to player's position, it "looks" to the right or left (used to move and attack in range)

  printGame();
  // Wait for key inputs for TURN_TIME_MS/4 ms
  getInput(TURN_TIME_MS/4);
  if (m_isLoaded) { m_isLoaded = false; return; } // Game has been paused, this game turn has no meaning anymore
  
  /*    Action 1 - Attacking - Done from closest to furthest troops
    We need to iterate backwards through player units vector, that's because new troops are pushed back so they are at the vectors end
  */
  std::vector<Unit*> units = playingP->getUnits();
  int closestUnitIndex = units.size() - 1;
  int waitingTimePerUnit = TURN_TIME_MS / (8 + units.size());
  
  for (int i = closestUnitIndex; i >= 0; i--) { // Iteration backwards from closest to furthest unit
    if (units[i]->getHp() > 0) {
      std::vector<std::pair<Unit*, bool> > targets; // To store available targets and if they're friendly or not
      int myPosition = units[i]->getPosition(); // Attacking unit's position
      std::pair<int, int> myRange = units[i]->getRange(); // Attacking unit's range
      bool canAttackEnemyBase = false;

      Unit* enemy; Unit* ally;
      for (int r = myPosition + myRange.first * direction; r != myPosition + (myRange.second + 1) * direction; r += direction) {
        if (r == 0 || r == MAP_SIZE - 1) canAttackEnemyBase = true;

        enemy = enemyPlayer->getUnitAtPosition(r);
        if (enemy && enemy->getHp() > 0) targets.push_back(std::make_pair(enemy, false));
        else {
          ally = playingP->getUnitAtPosition(r);
          if (ally && ally->getHp() > 0) targets.push_back(std::make_pair(ally, true));
        }
      }

      enemyPlayer->takeDamage(units[i]->action1(targets, canAttackEnemyBase)); // If units[i] attacks another unit, takeDamage will have 0 as parameter, otherwise, if units[i] attacks base, takeDamage will have unit's attack as parameter 
      printGame();
      getInput(waitingTimePerUnit); // Wait for key inputs for TURN_TIME_MS/(8 * nb units)
      if (m_isLoaded) { m_isLoaded = false; return; } // Game has been paused, this game turn has no meaning anymore
    }
  }

  killAllDeadUnits();
  units = playingP->getUnits(); // Actualize units vector (friendly units may have died)
  closestUnitIndex = units.size() - 1; // Same for closest unit index 
  
  /*    Action 2 and 3 - Moving and attacking - Done from farthest to closest troops
    We need to iterate forward through player units vector
  */
  for (int act = 2; act <= 3; act++) {
    for (int i = 0; i <= closestUnitIndex; i++) { // Iteration forward from farthest to nearest unit
      if (units[i]->getHp() > 0) {
        int newPosition = units[i]->getPosition() + direction;
        if (act == 2) { // If it's action 2
          if (!getUnitOwnerAtPosition(newPosition) || getUnitOwnerAtPosition(newPosition)->getUnitAtPosition(newPosition)->getHp() < 1) // Can move only if there's no unit in next cell 
            units[i]->action2(newPosition);
        } else { // It's action 3
          Unit* enemy = enemyPlayer->getUnitAtPosition(newPosition); // This time there's only one possible target because units than can attack are soldier that have range of 1
          bool canAttackEnemyBase = (newPosition == 0) || (newPosition == MAP_SIZE - 1);

          if (enemy && enemy->getHp() > 0) enemyPlayer->takeDamage(units[i]->action3(std::make_tuple(enemy, newPosition, false), canAttackEnemyBase));
          else {
            Unit* ally = playingP->getUnitAtPosition(newPosition);
            if (ally && ally->getHp() > 0) enemyPlayer->takeDamage(units[i]->action3(std::make_tuple(ally, newPosition, true), canAttackEnemyBase));
            else enemyPlayer->takeDamage(units[i]->action3(std::make_tuple(nullptr, newPosition, true), canAttackEnemyBase));
          }
        }
      }
      printGame();
      getInput(waitingTimePerUnit); // Wait for key inputs for TURN_TIME_MS/(8 * nb units) ms
      if (m_isLoaded) { m_isLoaded = false; return; } // Game has been paused, this game turn has no meaning anymore
    }
    killAllDeadUnits();
  }

  if (!m_playerTurn) { // it's P1 turn and they have pressed a key
    spawn(playingP->makeDecision(m_player1Key), *playingP);
    m_player1Key = '\0';
  }
  else { // it's P2 turn and they have pressed a key
    spawn(playingP->makeDecision(m_player2Key), *playingP);
    m_player2Key = '\0';
  }
  m_playerTurn = !m_playerTurn; // Change player turn
}

Player* Game::getUnitOwnerAtPosition(int position) {
  if (m_player1->getUnitAtPosition(position))
    return m_player1;
  if (m_player2->getUnitAtPosition(position))
    return m_player2;
  return nullptr;
}

void Game::killAllDeadUnits() {
  int reward = m_player1->killAllDeadUnits();
  m_player2->receiveMoney(reward);
  m_player2->receiveXp(reward);
  reward = m_player2->killAllDeadUnits();
  m_player1->receiveMoney(reward);
  m_player1->receiveXp(reward);
}

void Game::spawn(UnitNames unit, Player &p) {
  p.spawn(unit);
}



